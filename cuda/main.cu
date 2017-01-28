#include <stdio.h>
#include <stdint.h>
#include <cuda.h>
#include <omp.h>
__device__ uint32_t rotate_left(uint32_t x, uint32_t n) {
    return  (x << n) | (x >> (32-n));
}
__device__ uint32_t encrypt(uint32_t m, uint32_t key) {
    return (rotate_left(m, key&31) + key)^key;
}
__host__ uint32_t h_rotate_left(uint32_t x, uint32_t n) {
    return  (x << n) | (x >> (32-n));
}
__host__ uint32_t h_encrypt(uint32_t m, uint32_t key) {
    return (h_rotate_left(m, key&31) + key)^key;
}
#define MAXN 16777216
#define GPULOCAL 128
#define BLOCKSZ (1024)
__global__ void vecdot(uint32_t keyA, uint32_t keyB, uint32_t C[], int N) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int l = x * BLOCKSZ;
    int r = l + BLOCKSZ;
    uint32_t sum = 0;
    if (r > N)	r = N;
    for (int i = l; i < r; i++)
        sum += encrypt(i, keyA) * encrypt(i, keyB);
    C[x] = sum;
}

uint32_t hostC[MAXN / GPULOCAL];
#define CheckErr(status) { gpuAssert((status), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, int abort=true) {
   if (code != cudaSuccess) {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}
int main() {
    uint32_t N, keyA, keyB;
    uint32_t *cuArrC;
    cudaMalloc((void **)&cuArrC, MAXN/GPULOCAL*sizeof(uint32_t));
    while (scanf("%u %u %u", &N, &keyA, &keyB) == 3) {
        int M = (N + BLOCKSZ-1) / BLOCKSZ;
        int LOCAL = GPULOCAL;
        M = (M + LOCAL) / LOCAL * LOCAL;
        dim3 cuBlock(LOCAL);
        dim3 cuGrid(M/LOCAL);
        vecdot<<<cuGrid, cuBlock>>>(keyA, keyB, cuArrC, N);
        CheckErr(cudaGetLastError());
        cudaMemcpy(hostC, cuArrC, M*sizeof(uint32_t), cudaMemcpyDeviceToHost);
        uint32_t sum = 0;
#ifdef _OPENMP
        omp_set_num_threads(4);
#endif
        #pragma omp parallel for reduction(+: sum)
        for (int i = 0; i < M; i++)
            sum += hostC[i];
        printf("%u\n", sum);
    }
    cudaFree(cuArrC);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLOCK_SIZE 64 // 블록 크기 설정, 캐시 효율성을 높이기 위해 사용
#define M 1024
#define K 1024
#define N 1024

// 랜덤 실수 생성 함수
float gen_random_float(float min, float max) {
    return ((float) rand() / RAND_MAX) * (max - min) + min;
}

// 블록 단위로 행렬 곱셈을 수행하는 함수
void block_multiply(float* A, float* B, float* C, int row_start, int col_start, int k_start) {
    // 블록 내의 각 원소에 대해 곱셈을 수행
    for (int i = row_start; i < row_start + BLOCK_SIZE; i++) {
        for (int j = col_start; j < col_start + BLOCK_SIZE; j++) {
            for (int k = k_start; k < k_start + BLOCK_SIZE; k++) {
                // 행렬 A의 i번째 행과 B의 j번째 열을 곱하고 C에 더함
                C[i * N + j] += A[i * K + k] * B[k * N + j];
            }
        }
    }
}

int main() {
    static float A[M * K], B[K * N], C[M * N] = {0}; // 행렬 A, B, C 정의 및 초기화

    srand((unsigned int)time(NULL)); // 랜덤 시드 설정

    // 배열 A와 B 초기화
    for (int i = 0; i < M * K; i++) {
        A[i] = gen_random_float(0.0f, 0.1f); // A의 각 원소를 랜덤 값으로 초기화
    }
    for (int i = 0; i < K * N; i++) {
        B[i] = gen_random_float(0.0f, 0.1f); // B의 각 원소를 랜덤 값으로 초기화
    }

    // 블록 단위로 행렬 곱셈 수행
    // M, N, K 크기의 행렬을 BLOCK_SIZE 크기의 블록으로 나누어 처리
    for (int i = 0; i < M; i += BLOCK_SIZE) { // 행단위로 블록 이동
        for (int j = 0; j < N; j += BLOCK_SIZE) { // 열단위로 블록 이동
            for (int k = 0; k < K; k += BLOCK_SIZE) { // k단위로 블록 이동
                block_multiply(A, B, C, i, j, k); // 각 블록에 대해 곱셈 수행
            }
        }
    }

    // 결과 출력 (C[0]은 C 행렬의 첫 번째 원소)
    printf("%lf\n", C[0]);
    return 0;
}


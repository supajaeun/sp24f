#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned char* pointer;

bool is64bit() {
    return (sizeof(void*) == 8); // 포인터 크기가 8이면 true 반환
}

bool isBigEndian() {
    int num = 1;
    return (*(char*)&num == 0); // 첫번째 바이트가 0이면 big endian
}

// 리틀 엔디언 바이트 출력 함수
void le_show_bytes(pointer start, size_t len) {
    for (int i = len - 1; i >= 0; i--) // 역순으로 출력
        printf("%.2X", start[i]);
    printf("\n");
}

// 빅 엔디언 바이트 출력 함수
void be_show_bytes(pointer start, size_t len) {
    for (size_t i = 0; i < len; i++)
        printf("%.2X", start[i]);
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: ./decfp2hex number\n");
        exit(0);
    }

    double a = atof(argv[1]); // 실수를 double 타입으로 변환

    // 아키텍처 출력
    printf("ARCH=%d\n", is64bit() ? 64 : 32);

    // 엔디안 방식 출력
    printf("ORDERING=%s\n", isBigEndian() ? "BIG_ENDIAN" : "LITTLE_ENDIAN");

    // MYANS: DEC 및 HEX 출력
    printf("MYANS: DEC=%.3f HEX=", a); // 소수점 3자리까지 출력
    if (isBigEndian()) {
        be_show_bytes((pointer)&a, sizeof(double)); // 빅 엔디언 방식 출력
    } else {
        le_show_bytes((pointer)&a, sizeof(double)); // 리틀 엔디언 방식 출력 (역순으로 출력)
    }

    // 정답 확인용 출력

    return 0;
}

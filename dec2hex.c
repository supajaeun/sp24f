#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned char* pointer;

bool is64bit() {
    return (sizeof(void*) == 8); //sizeof(void*) : 포인터 값의 크기
     // 포인터 크기가 8이면 true 반환
}

bool isBigEndian() {
    int num = 1;
    return (*(char*)&num == 0); // 첫번재 바이트가 0이면 big endian
    // &num은 int* 타입의 포인터
    // c에서 char 타입은 항상 1바이트, 즉 포인터를 char*로 캐스팅하면 메모리를 1byte씩 다룰 수 있음
    //  즉 &num을 char*로 타입캐스팅 -> *(역참조)하여 첫 번째 byte에 접근
}


// 리틀 엔디언 바이트 출력 함수 : 리틀 엔디언 시스템에서는 역순으로 출력(빅 엔디언 방식 출력)
void le_show_bytes(pointer start, size_t len)
{
    for (int i = len - 1; i >= 0; i--) // 역순으로 출력 (len-1 부터 0까지 감소하며 바이트 출력)
        printf("%.2X", start[i]); // .2X 두자리 16진수 형식으로 출력
        //start는 포인터이므로, start[i] 는 i번째 바이트 값을 가져옴
    
    printf("\n");
}

// 빅 엔디언 바이트 출력 함수 (빅 엔디언 시스템에서는 그대로 출력)
void be_show_bytes(pointer start, size_t len)
{
    // 빅 엔디언 시스템에서는 그대로 출력 (빅 엔디언 방식 출력)
    for (size_t i = 0; i < len; i++)
        printf("%.2X", start[i]);
    printf("\n");
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: ./a.out number\n");
        exit(0);
    }

    unsigned int a = atoi(argv[1]);

    // 아키텍처 출력
    printf("ARCH=%d\n", is64bit() ? 64 : 32);

    // 엔디안 방식 출력
    printf("ORDERING=%s\n", isBigEndian() ? "BIG_ENDIAN" : "LITTLE_ENDIAN");

    // MYANS: DEC 및 HEX 출력
    printf("MYANS: DEC=%d HEX=", a);
    if (isBigEndian()) {
        be_show_bytes((pointer)&a, sizeof(unsigned int));  // 빅 엔디언 방식 출력
    } else {
        le_show_bytes((pointer)&a, sizeof(unsigned int));   // 리틀 엔디언 방식 출력 (역순으로 출력)
    }

    // 정답 확인용 출력
    printf("CHECK: DEC=%d HEX=%.8X\n", a, a);

    return 0;
}

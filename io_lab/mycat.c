#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 16

int main(int argc, char* argv[]) {
    int ifd, ofd;
    char* buff;
    ssize_t nreads, nwrites;

    // 인자가 충분히 있는지 확인
    if (argc < 2) {
        fprintf(stdout, "No arguments\n");
        return 0;
    } else if (argc == 2) {
        // 케이스: 표준 출력으로 출력하기
        // 입력 파일을 읽기 전용 모드로 열기
        ifd = open(argv[1], O_RDONLY);
        if (ifd < 0) {
            fprintf(stderr, "입력 파일 열기 실패: %s\n", strerror(errno));
            return -1;
        }

        // 버퍼 메모리 할당
        if (!(buff = (char*) malloc(sizeof(char) * BUFF_SIZE))) {
            fprintf(stderr, "메모리 할당 실패\n");
            close(ifd);
            return -1;
        }

        // 입력 파일에서 읽어 표준 출력으로 쓰기
        while ((nreads = read(ifd, buff, BUFF_SIZE)) > 0) {
            nwrites = write(STDOUT_FILENO, buff, nreads);
            if (nwrites != nreads) {
                // 쓰기 오류 처리
                fprintf(stderr, "쓰기 오류: %s\n", strerror(errno));
                free(buff);
                close(ifd);
                return -1;
            }
        }

        // 읽기 오류 확인
        if (nreads < 0) {
            fprintf(stderr, "읽기 오류: %s\n", strerror(errno));
        }

        // 할당된 버퍼 해제 및 입력 파일 닫기
        free(buff);
        close(ifd);
    } else if (argc == 4 && !strcmp(argv[2], "into")) {
        // 케이스: 리다이렉션 파일로 출력하기
        // 입력 파일을 읽기 전용 모드로 열기
        ifd = open(argv[1], O_RDONLY);
        if (ifd < 0) {
            fprintf(stderr, "입력 파일 열기 실패: %s\n", strerror(errno));
            return -1;
        }

        // 출력 파일을 쓰기 전용으로 열거나 생성, 필요한 권한 설정
        ofd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (ofd < 0) {
            fprintf(stderr, "출력 파일 열기 실패: %s\n", strerror(errno));
            close(ifd);
            return -1;
        }

        // 버퍼 메모리 할당
        if (!(buff = (char*) malloc(sizeof(char) * BUFF_SIZE))) {
            fprintf(stderr, "메모리 할당 실패\n");
            close(ifd);
            close(ofd);
            return -1;
        }

        // 입력 파일에서 읽어 출력 파일로 쓰기
        while ((nreads = read(ifd, buff, BUFF_SIZE)) > 0) {
            nwrites = write(ofd, buff, nreads);
            if (nwrites != nreads) {
                // 쓰기 오류 처리
                fprintf(stderr, "쓰기 오류: %s\n", strerror(errno));
                free(buff);
                close(ifd);
                close(ofd);
                return -1;
            }
        }

        // 읽기 오류 확인
        if (nreads < 0) {
            fprintf(stderr, "읽기 오류: %s\n", strerror(errno));
        }

        // 할당된 버퍼 해제 및 파일 닫기
        free(buff);
        close(ifd);
        close(ofd);
    } else {
        // 케이스: 잘못된 인자 처리
        fprintf(stdout, "Wrong arguments\n");
        return 0;
    }

    return 0;
}

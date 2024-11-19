#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 1024

int main(int argc, char **argv) {
    int clientfd, port;
    struct sockaddr_in serveraddr;
    char *host, buf[MAXLINE];

    host = argv[1];
    port = atoi(argv[2]);

    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons((unsigned short)port);
    serveraddr.sin_addr.s_addr = inet_addr(host);

    if (connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("connect");
        exit(1);
    }

    int rbytes, wbytes;
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        wbytes = write(clientfd, buf, strlen(buf)); // 사용자가 입력한 데이터를 서버로 전송.
        // clientfd는 서버와의 연결을 나타내는 파일 디스크립터
        if (wbytes < strlen(buf))
            printf("Failed to send message\n");
        rbytes = read(clientfd, buf, MAXLINE); //서버로부터 다시 돌아온 데이터를 읽어옴
        fputs(buf, stdout);
    }

    close(clientfd);
    return 0;
}

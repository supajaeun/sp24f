#include <stdio.h>      // fprintf, stdout
#include <sys/types.h>  // pid_t
#include <unistd.h>     // fork, sleep
#include <signal.h>     // signal, SIGCHLD
#include <sys/wait.h>   // wait
#include <stdlib.h>     // exit

#define NUM 8  // 자식 프로세스의 수

int ccount = 0;  // 자식 프로세스 카운트 변수

// SIGCHLD 시그널을 처리하는 핸들러
void child_handler1(int sig) {
    int status;
    pid_t pid = wait(&status);  // 종료된 자식 프로세스 대기
    ccount--;  // 남은 자식 프로세스 수 감소
    fprintf(stdout, "Received signal %d from process %d\n", sig, pid);
}

void child_handler2(int sig) {
    fprintf(stdout, "child_handler2 ... \n"); // 핸들러 호출 시 메시지 출력
    int status; // 자식 프로세스 종료 상태를 저장할 변수
    pid_t pid;  // 종료된 자식 프로세스의 PID를 저장할 변수

    // 종료된 모든 자식 프로세스를 처리하기 위한 반복문
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        ccount--; // 자식 프로세스 카운트 감소
        fprintf(stdout, "Received signal %d from process %d\n", sig, pid); // 종료된 자식 프로세스 정보 출력
    }
}

int main(int argc, char* argv[]) {
    pid_t pid[NUM];  // 자식 프로세스의 PID 배열
    int i, status;

    ccount = NUM;  // 자식 프로세스 개수를 초기화

    // SIGCHLD 시그널 핸들러 등록s
    signal(SIGCHLD, child_handler1);

    // 자식 프로세스 생성
    for (i = 0; i < NUM; i++) {
        if ((pid[i] = fork()) == 0) {  // 자식 프로세스라면
            sleep(2);  // 2초 대기
            exit(0);   // 종료
        }
    }

    // 모든 자식 프로세스가 종료될 때까지 대기
    while (ccount > 0) {
        pause();  // 시그널이 도착할 때까지 대기
    }

    return 0;
}

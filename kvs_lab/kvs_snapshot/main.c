#include "kvs.h"
#include <stdio.h>
#include <string.h>

int main() {
    kvs_t* kvs = open_kvs(); // 수정된 함수 이름 사용
    if (!kvs) {
        printf("Failed to open KVS\n");
        return -1;
    }

    FILE* workloadFile = fopen("cluster004.trc", "r");
    if (!workloadFile) {
        perror("Failed to open workload file");
        return -1;
    }

    char op[10], key[100], value[VALUE_MAX]; // VALUE_MAX 사용
    while (fscanf(workloadFile, "%9[^,],%99[^,],%9999[^\n]\n", op, key, value) != EOF) {
        if (strcmp(op, "set") == 0) {
            put(kvs, key, value);
        }
    }
    fclose(workloadFile);

    // in-memory 데이터셋 준비 완료
    printf("In-memory dataset prepared\n");

    // KVS 상태를 스냅샷으로 저장
    if (do_snapshot(kvs, "kvs.img") == 0) {
        printf("Snapshot saved successfully\n");
    } else {
        printf("Failed to save snapshot\n");
    }

    close_kvs(kvs); // KVS 종료

    // KVS 복구 테스트
    kvs = open_kvs(); // 새로 KVS 열기
    if (do_recovery(kvs, "kvs.img") == 0) {
        printf("Recovery successful\n");
    } else {
        printf("Failed to recover from snapshot\n");
    }

    // 복구된 데이터 확인
    char* value1 = get(kvs, "tweet55");
    char* value2 = get(kvs, "tweet13843");
    char* value3 = get(kvs, "tweet3482");
    
    printf("Recovered value for 'tweet55': %s\n\n", value1);
    printf("Recovered value for 'tweet13843': %s\n\n", value2);
    printf("Recovered value for 'tweet3482': %s\n\n", value3);


    close_kvs(kvs);
    return 0;
}


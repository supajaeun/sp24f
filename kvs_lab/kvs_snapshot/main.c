#include "kvs.h"

int main() {
    kvs_t* kvs = open_kvs();

    if (!kvs) {
        printf("Failed to open KVS\n");
        return -1;
    }

    // 테스트 워크로드 실행: cluster004.trc 파일 읽기
    FILE* workloadFile = fopen("cluster004.trc", "r");
    if (!workloadFile) {
        printf("Workload file not found\n");
        close_kvs(kvs);
        return -1;
    }

    char op[10], key[100], value[100];
    while (fscanf(workloadFile, "%9[^,],%99[^,],%99s\n", op, key, value) != EOF) {
    // 기존 key[100], value[100] 배열 크기에 맞게 조정
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
    char* recovered_value = get(kvs, "tweet17");
    if (recovered_value) {
        printf("Recovered value for 'tweet17': %s\n", recovered_value);
    } else {
        printf("Key 'tweet17' not found\n");
    }

    close_kvs(kvs);
    return 0;
}

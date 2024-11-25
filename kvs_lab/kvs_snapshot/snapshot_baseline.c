#include <stdio.h>
#include <unistd.h> // for fsync
#include <string.h>
#include "kvs.h"

int do_snapshot(kvs_t* kvs, const char* filepath) {
    if (!kvs || !filepath) return -1;

    FILE* file = fopen(filepath, "w");
    if (!file) {
        perror("Failed to open file for snapshot");
        return -1;
    }

    // KVS 데이터 저장
    node_t* node = kvs->header->next[0];
    while (node) {
        fprintf(file, "%s,%s\n", node->key, node->value);
        node = node->next[0];
    }

    // fflush로 OS 버퍼를 비우고
    if (fflush(file) != 0) {
        perror("Failed to flush file");
        fclose(file);
        return -1;
    }

    // fsync로 디스크 기록 보장
    if (fsync(fileno(file)) != 0) {
        perror("Failed to sync file to disk");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

int do_recovery(kvs_t* kvs, const char* filepath) {
    if (!kvs || !filepath) return -1;

    FILE* file = fopen(filepath, "r");
    if (!file) {
        perror("Failed to open file for recovery");
        return -1;
    }

    char key[100], value[VALUE_MAX]; // VALUE_MAX 상수 사용
    while (fscanf(file, "%99[^,],%4999[^\n]\n", key, value) != EOF) {
        // 입력 크기 검증
        if (strlen(key) >= sizeof(key) || strlen(value) >= VALUE_MAX) {
            fprintf(stderr, "Invalid data in snapshot file\n");
            continue; // 잘못된 데이터를 건너뜀
        }
        put(kvs, key, value); // 복구된 데이터를 KVS에 추가
    }

    fclose(file);
    return 0;
}

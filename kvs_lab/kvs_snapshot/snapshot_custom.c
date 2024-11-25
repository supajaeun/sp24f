#include <fcntl.h>    // for open
#include <unistd.h>   // for write, read, fsync, close
#include <string.h>   // for strlen
#include <stdlib.h>   // for malloc, free
#include <stdio.h>    // for perror
#include "kvs.h"

int do_snapshot(kvs_t* kvs, const char* filepath) {
    if (!kvs || !filepath) return -1;

    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Failed to open file for snapshot");
        return -1;
    }

    // KVS 데이터를 순회하며 파일에 저장
    node_t* node = kvs->header->next[0];
    char buffer[256];
    while (node) {
        int len = snprintf(buffer, sizeof(buffer), "%s,%s\n", node->key, node->value);
        if (write(fd, buffer, len) < 0) {
            perror("Failed to write to file");
            close(fd);
            return -1;
        }
        node = node->next[0];
    }

    // fsync로 디스크 기록 보장
    if (fsync(fd) != 0) {
        perror("Failed to sync file to disk");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

int do_recovery(kvs_t* kvs, const char* filepath) {
    if (!kvs || !filepath) return -1;

    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file for recovery");
        return -1;
    }

    char buffer[256];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; // 문자열 끝 처리

        // 각 라인을 처리
        char* line = strtok(buffer, "\n");
        while (line) {
            char key[100], value[100];
            if (sscanf(line, "%99[^,],%99s", key, value) == 2) {
                put(kvs, key, value); // 복구된 데이터를 KVS에 추가
            }
            line = strtok(NULL, "\n");
        }
    }

    if (bytes_read < 0) {
        perror("Failed to read file");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

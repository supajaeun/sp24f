#include <fcntl.h>    // for open, O_CREAT, O_WRONLY, etc.
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

    // KVS 데이터 저장
    node_t* node = kvs->header->next[0];
    char buffer[VALUE_MAX]; // 충분히 큰 버퍼
    while (node) {
        if (strlen(node->value) >= VALUE_MAX) {
            node = node->next[0];
            continue; // 너무 큰 값은 저장하지 않음
        }
        int len = snprintf(buffer, sizeof(buffer), "%s,%s\n", node->key, node->value);
        if (len < 0 || len >= (int)sizeof(buffer)) {
            fprintf(stderr, "Error formatting snapshot data\n");
            close(fd);
            return -1;
        }
        if (write(fd, buffer, len) < 0) {
            perror("Failed to write to snapshot file");
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

    char buffer[VALUE_MAX]; // 충분히 큰 버퍼
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; // 문자열 끝 처리

        char* line = strtok(buffer, "\n");
        while (line) {
            char key[100], value[VALUE_MAX];
            if (sscanf(line, "%99[^,],%4999[^\n]", key, value) == 2) {
                if (strlen(value) >= VALUE_MAX) {
                    fprintf(stderr, "Value exceeds limit during recovery: %s\n", value);
                } else {
                    put(kvs, key, value);
                }
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

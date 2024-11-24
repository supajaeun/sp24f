#include "kvs.h"
#include <unistd.h>  // fsync 함수 사용을 위한 헤더 파일

int do_snapshot(kvs_t* kvs) {
    if (!kvs) {
        return -1;
    }

    FILE *snapshot_file = fopen("kvs.img", "w");
    if (!snapshot_file) {
        printf("Failed to open snapshot file for writing\n");
        return -1;
    }

    node_t* node = kvs->header->next[0];
    while (node) {
        fprintf(snapshot_file, "%s,%s\n", node->key, node->value);
        node = node->next[0];
    }

    fflush(snapshot_file);  // Ensure data is flushed to disk
    fsync(fileno(snapshot_file)); // Use fsync to ensure all data is physically written to disk
    fclose(snapshot_file);

    return 0;
}

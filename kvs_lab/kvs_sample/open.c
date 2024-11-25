#include "kvs.h"

kvs_t* open() {
    kvs_t* kvs = (kvs_t*)malloc(sizeof(kvs_t));
    if (!kvs) {
        printf("Failed to allocate memory for kvs\n");
        return NULL;
    }

    kvs->header = (node_t*)malloc(sizeof(node_t));
    if (!kvs->header) {
        printf("Failed to allocate memory for header node\n");
        free(kvs);
        return NULL;
    }

    for (int i = 0; i < MAX_LEVEL; i++) {
        kvs->header->forward[i] = NULL;
    }

    kvs->header->key[0] = '\0'; // header 노드의 키는 빈 값으로 둡니다.
    kvs->header->value = NULL;
    kvs->level = 1;
    kvs->items = 0;

    printf("Open: kvs has %d items\n", kvs->items);
    return kvs;
}

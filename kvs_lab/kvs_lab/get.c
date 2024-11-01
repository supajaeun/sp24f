#include "kvs.h"

char* get(kvs_t* kvs, const char* key) {
    node_t* current = kvs->header;

    // 각 레벨에서 검색
    for (int i = kvs->level - 1; i >= 0; i--) {
        while (current->forward[i] != NULL && strcmp(current->forward[i]->key, key) < 0) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];

    // 키가 존재하면 값 반환
    if (current != NULL && strcmp(current->key, key) == 0) {
        char* value = (char*)malloc(strlen(current->value) + 1);
        strcpy(value, current->value);
        return value;
    }
    return NULL;
}

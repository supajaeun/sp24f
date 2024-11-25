#include "kvs.h"

// 난수로 레벨을 결정
int random_level() {
    int level = 1;
    while (rand() < RAND_MAX / 2 && level < MAX_LEVEL)
        level++;
    return level;
}

int put(kvs_t* kvs, const char* key, const char* value) {
    node_t* update[MAX_LEVEL];
    node_t* current = kvs->header;

    // 각 레벨에서 삽입 위치를 찾기
    for (int i = kvs->level - 1; i >= 0; i--) {
        while (current->forward[i] != NULL && strcmp(current->forward[i]->key, key) < 0) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];

    // 키가 존재하면 값 갱신
    if (current != NULL && strcmp(current->key, key) == 0) {
        free(current->value);
        current->value = strdup(value);
        return 0;
    }

    // 새로운 노드 삽입
    int level = random_level();
    if (level > kvs->level) {
        for (int i = kvs->level; i < level; i++) {
            update[i] = kvs->header;
        }
        kvs->level = level;
    }

    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    strcpy(new_node->key, key);
    new_node->value = strdup(value);
    for (int i = 0; i < level; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
    kvs->items++;
    return 0;
}

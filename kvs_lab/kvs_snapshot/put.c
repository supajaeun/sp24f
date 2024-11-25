#include "kvs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int put(kvs_t* kvs, const char* key, const char* value) {
    node_t* prev_node[MAX_LEVEL]; // MAX_LEVEL = 16
    node_t* node = kvs->header;

    // 키 값이 너무 큰 경우 처리
    if (strlen(key) >= sizeof(node->key)) {
        fprintf(stderr, "Key is too large\n");
        return -1;
    }

    for (int i = kvs->kvs_mx_level; i >= 0; i--) { 
        while (node->next[i] && strcmp(node->next[i]->key, key) < 0) {
            node = node->next[i];
        }
        prev_node[i] = node;
    }
    node = node->next[0];

    if (node && strcmp(node->key, key) == 0) { // 이미 해당 노드가 있다면 값을 업데이트
        free(node->value);
        node->value = strdup(value);
        return 0;
    } else {
        int level = rand_lv(); // 랜덤 레벨 생성
        if (level > kvs->kvs_mx_level) { 
            for (int i = kvs->kvs_mx_level + 1; i <= level; i++) {
                prev_node[i] = kvs->header;
            }
            kvs->kvs_mx_level = level;
        }
        node = (node_t*)malloc(sizeof(node_t));
        if (!node) {
            fprintf(stderr, "Failed to allocate memory for new node\n");
            return -1;
        }
        strcpy(node->key, key);
        node->value = strdup(value);
        if (!node->value) {
            fprintf(stderr, "Failed to allocate memory for value\n");
            free(node);
            return -1;
        }

        // 새로운 노드를 삽입
        for (int i = 0; i <= level; i++) {
            node->next[i] = prev_node[i]->next[i];
            prev_node[i]->next[i] = node;
        }
        kvs->items++;
        return 0;
    }
}

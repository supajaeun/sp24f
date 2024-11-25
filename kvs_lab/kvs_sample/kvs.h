#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEVEL 6 // Skip list의 최대 레벨

struct node {
    char key[100];
    char* value;
    struct node* forward[MAX_LEVEL]; // 각 레벨을 가리키는 포인터
};    
typedef struct node node_t;

struct kvs {
    struct node* header; // Skip list의 시작 노드
    int level; // 현재 Skip list의 레벨
    int items; // 데이터의 개수
};
typedef struct kvs kvs_t;

kvs_t* open();
int close(kvs_t* kvs);
int put(kvs_t* kvs, const char* key, const char* value);
char* get(kvs_t* kvs, const char* key);

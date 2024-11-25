#ifndef KVS_H
#define KVS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// 상수 정의
#define MAX_LEVEL 16
#define VALUE_MAX 5000  // value 크기 조정이 가능한 상수

struct node {
    char key[100];
    char *value;
    struct node* next[MAX_LEVEL];
};
typedef struct node node_t;

struct kvs {
    struct node* header;  // 데이터베이스
    int items;            // 데이터 개수
    int kvs_mx_level;     // 현재 KVS의 최대 레벨
};
typedef struct kvs kvs_t;

// 함수 선언
kvs_t* open_kvs(); // 기존 open을 open_kvs로 변경
int close_kvs(kvs_t* kvs); // 기존 close를 close_kvs로 변경

int put(kvs_t* kvs, const char* key, const char* value);
char* get(kvs_t* kvs, const char* key);
int rand_lv();
int do_snapshot(kvs_t* kvs, const char* filepath);
int do_recovery(kvs_t* kvs, const char* filepath);

#endif // KVS_H

#include "kvs.h"

int main() {
    kvs_t* kvs = open();
    if (!kvs) {
        printf("Failed to open kvs\n");
        return -1;
    }

    FILE* file = fopen("query.dat", "r");
    FILE* answer = fopen("answer.dat", "w");
    char operation[10], key[100], value[100];

    while (fscanf(file, "%[^,],%[^,],%s\n", operation, key, value) != EOF) {
        if (strcmp(operation, "set") == 0) {
            put(kvs, key, value);
        } else if (strcmp(operation, "get") == 0) {
            char* result = get(kvs, key);
            if (result) {
                fprintf(answer, "%s\n", result);
                free(result);
            } else {
                fprintf(answer, "-1\n");
            }
        }
    }

    fclose(file);
    fclose(answer);
    close(kvs);

    return 0;
}

#include "kvs.h"

int do_recovery(kvs_t* kvs) {
    if (!kvs) {
        return -1;
    }

    FILE *snapshot_file = fopen("kvs.img", "r");
    if (!snapshot_file) {
        printf("Failed to open snapshot file for reading\n");
        return -1;
    }

    char key[100];
    char value[100];
    while (fscanf(snapshot_file, "%[^,],%s\n", key, value) != EOF) {
        put(kvs, key, value);
    }

    fclose(snapshot_file);

    return 0;
}

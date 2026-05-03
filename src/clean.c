#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bsql.h"

static int remove_if_exists(const char *path) {
    if (access(path, F_OK) != 0) return 0;

    if (remove(path) != 0) {
        perror(path);
        return 1;
    }

    printf("Removed: %s\n", path);
    return 0;
}

int bsql_clean(void) {
    const char *home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "bsql clean error: HOME not set\n");
        return 1;
    }

    char index_path[4096];

    snprintf(
        index_path,
        sizeof(index_path),
        "%s/.local/share/bsql/index/bsql.index.jsonl",
        home
    );

    printf("BetterSQL clean\n\n");
    printf("Safe mode: removing cache/index files only.\n");
    printf("Real files are never touched.\n");
    printf("Sidecars are never touched.\n\n");

    int rc = remove_if_exists(index_path);

    if (rc == 0) {
        printf("\nClean complete.\n");
        bsql_log_event("clean", "ok", "Removed cache/index files.");
    } else {
        bsql_log_event("clean", "error", "Failed to remove one or more cache/index files.");
    }

    return rc;
}

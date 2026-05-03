#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "bsql.h"

static uint64_t fnv1a64(const char *s) {
    uint64_t h = 1469598103934665603ULL;

    while (*s) {
        h ^= (unsigned char)(*s++);
        h *= 1099511628211ULL;
    }

    return h;
}

int bsql_id(const char *path) {
    uint64_t id = fnv1a64(path);

    printf("BetterSQL id\n\n");
    printf("Path: %s\n", path);
    printf("ID: bsql_%016llx\n", (unsigned long long)id);
    printf("\nID V0: deterministic hash of path.\n");
    printf("Future: stable sidecar identity that survives moves.\n");

    bsql_log_event("id", "ok", "Printed BetterSQL ID.");
    return 0;
}

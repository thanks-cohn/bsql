#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "bsql.h"

static uint64_t fnv1a64(const char *s) {
    uint64_t h = 1469598103934665603ULL;

    while (*s) {
        h ^= (unsigned char)(*s++);
        h *= 1099511628211ULL;
    }

    return h;
}

static int print_index_record(const char *path) {
    const char *home = getenv("HOME");
    if (!home) return 0;

    char index_path[4096];
    snprintf(index_path, sizeof(index_path), "%s/.local/share/bsql/index/bsql.index.jsonl", home);

    FILE *f = fopen(index_path, "r");
    if (!f) {
        printf("Indexed record: missing index\n");
        return 0;
    }

    char line[16384];
    int found = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, path)) {
            printf("Indexed record:\n%s", line);
            found = 1;
            break;
        }
    }

    fclose(f);

    if (!found) {
        printf("Indexed record: not found\n");
    }

    return found;
}

static int print_sidecar(const char *path) {
    char sidecar[8192];
    snprintf(sidecar, sizeof(sidecar), "%s.wmeta.json", path);

    FILE *f = fopen(sidecar, "r");
    if (!f) {
        printf("Sidecar: not found\n");
        return 0;
    }

    printf("Sidecar: %s\n", sidecar);
    printf("Sidecar content:\n");

    int c;
    while ((c = fgetc(f)) != EOF) {
        fputc(c, stdout);
    }

    fclose(f);
    return 1;
}

int bsql_context(const char *path) {
    uint64_t id = fnv1a64(path);

    printf("BetterSQL context\n\n");
    printf("Path: %s\n", path);
    printf("Exists: %s\n", access(path, F_OK) == 0 ? "yes" : "no");
    printf("ID: bsql_%016llx\n", (unsigned long long)id);
    printf("\n");

    print_index_record(path);
    printf("\n");

    print_sidecar(path);
    printf("\n");

    printf("History: V0 current path only\n");
    printf("Context V0: path + id + index + sidecar\n");

    bsql_log_event("context", "ok", "Printed file context.");
    return 0;
}

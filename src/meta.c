#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsql.h"

int bsql_meta(const char *path) {
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "bsql meta error: HOME not set\n");
        return 1;
    }

    char index_path[4096];
    snprintf(index_path, sizeof(index_path),
        "%s/.local/share/bsql/index/bsql.index.jsonl", home);

    FILE *f = fopen(index_path, "r");
    if (!f) {
        fprintf(stderr, "bsql meta error: index not found. Run: bsql scan <path>\n");
        return 1;
    }

    char line[16384];
    int found = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, path)) {
            printf("%s", line);
            found = 1;
            break;
        }
    }

    fclose(f);

    if (!found) {
        fprintf(stderr, "bsql meta: not found: %s\n", path);
        return 1;
    }

    bsql_log_event("meta", "ok", "Fetched metadata for file.");
    return 0;
}

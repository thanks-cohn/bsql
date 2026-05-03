#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsql.h"

int bsql_history(const char *path) {
    const char *home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "bsql history error: HOME not set\n");
        return 1;
    }

    char index_path[4096];
    snprintf(index_path, sizeof(index_path), "%s/.local/share/bsql/index/bsql.index.jsonl", home);

    FILE *f = fopen(index_path, "r");
    if (!f) {
        fprintf(stderr, "bsql history error: index not found. Run: bsql scan <path> or bsql rebuild <path>\n");
        return 1;
    }

    char line[16384];
    int found = 0;

    printf("BetterSQL history\n\n");
    printf("Target: %s\n\n", path);

    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, path)) {
            printf("Current indexed record:\n");
            printf("%s", line);
            found = 1;
            break;
        }
    }

    fclose(f);

    if (!found) {
        fprintf(stderr, "bsql history: no indexed history found for: %s\n", path);
        return 1;
    }

    printf("\nHistory V0: current index record only.\n");
    printf("Future: stable IDs, path_history, moves, renames.\n");

    bsql_log_event("history", "ok", "Printed current indexed history.");
    return 0;
}

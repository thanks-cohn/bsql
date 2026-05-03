#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bsql.h"

static int extract_json_string(const char *line, const char *key, char *out, size_t n) {
    char pattern[128];
    snprintf(pattern, sizeof(pattern), "\"%s\":\"", key);

    const char *p = strstr(line, pattern);
    if (!p) return 0;

    p += strlen(pattern);

    size_t i = 0;
    while (*p && *p != '"' && i + 1 < n) {
        out[i++] = *p++;
    }

    out[i] = '\0';
    return 1;
}

int bsql_missing(void) {
    const char *home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "bsql missing error: HOME not set\n");
        return 1;
    }

    char index_path[4096];
    snprintf(index_path, sizeof(index_path), "%s/.local/share/bsql/index/bsql.index.jsonl", home);

    FILE *f = fopen(index_path, "r");
    if (!f) {
        fprintf(stderr, "bsql missing error: index not found. Run: bsql scan <path> or bsql compile <path>\n");
        return 1;
    }

    char line[16384];
    long checked = 0;
    long missing = 0;

    printf("BetterSQL missing\n\n");

    while (fgets(line, sizeof(line), f)) {
        char path[4096] = "";

        if (!extract_json_string(line, "path", path, sizeof(path))) {
            continue;
        }

        checked++;

        if (access(path, F_OK) != 0) {
            missing++;
            printf("[%ld] %s\n", missing, path);
        }
    }

    fclose(f);

    printf("\nChecked: %ld\n", checked);
    printf("Missing: %ld\n", missing);

    bsql_log_event("missing", "ok", "Checked index for missing files.");
    return 0;
}

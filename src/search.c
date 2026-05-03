#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bsql.h"

static void lower_copy(char *dst, const char *src, size_t n) {
    size_t i = 0;
    for (; src[i] && i + 1 < n; i++) {
        dst[i] = (char)tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

int bsql_search(const char *query) {
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "bsql search error: HOME not set\n");
        return 1;
    }

    char index_path[4096];
    snprintf(index_path, sizeof(index_path), "%s/.local/share/bsql/index/bsql.index.jsonl", home);

    FILE *f = fopen(index_path, "r");
    if (!f) {
        fprintf(stderr, "bsql search error: index not found. Run: bsql scan <path>\n");
        return 1;
    }

    char q[1024];
    lower_copy(q, query, sizeof(q));

    char line[16384];
    long matches = 0;

    printf("BetterSQL search: %s\n\n", query);

    while (fgets(line, sizeof(line), f)) {
        char lowered[16384];
        lower_copy(lowered, line, sizeof(lowered));

        if (strstr(lowered, q)) {
            matches++;
            printf("[%ld] %s", matches, line);
        }
    }

    fclose(f);

    printf("\nMatches: %ld\n", matches);

    bsql_log_event("search", "ok", "Searched JSONL index.");
    return 0;
}

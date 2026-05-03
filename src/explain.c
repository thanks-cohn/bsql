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

static int contains_ci(const char *haystack, const char *needle) {
    char h[16384];
    char n[1024];

    lower_copy(h, haystack, sizeof(h));
    lower_copy(n, needle, sizeof(n));

    return strstr(h, n) != NULL;
}

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

int bsql_explain(const char *query) {
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "bsql explain error: HOME not set\n");
        return 1;
    }

    char index_path[4096];
    snprintf(index_path, sizeof(index_path), "%s/.local/share/bsql/index/bsql.index.jsonl", home);

    FILE *f = fopen(index_path, "r");
    if (!f) {
        fprintf(stderr, "bsql explain error: index not found. Run: bsql scan <path>\n");
        return 1;
    }

    char line[16384];
    long matches = 0;

    printf("BetterSQL explain: %s\n\n", query);

    while (fgets(line, sizeof(line), f)) {
        if (!contains_ci(line, query)) continue;

        char path[4096] = "";
        char name[1024] = "";
        char kind[128] = "";

        extract_json_string(line, "path", path, sizeof(path));
        extract_json_string(line, "name", name, sizeof(name));
        extract_json_string(line, "kind", kind, sizeof(kind));

        matches++;

        printf("[%ld] %s\n", matches, path[0] ? path : "(unknown path)");
        printf("Matched fields:\n");

        int field_matches = 0;

        if (path[0] && contains_ci(path, query)) {
            printf("  - path: %s\n", path);
            field_matches++;
        }

        if (name[0] && contains_ci(name, query)) {
            printf("  - name: %s\n", name);
            field_matches++;
        }

        if (kind[0] && contains_ci(kind, query)) {
            printf("  - kind: %s\n", kind);
            field_matches++;
        }

        if (!field_matches) {
            printf("  - raw record matched\n");
        }

        printf("\n");
    }

    fclose(f);

    printf("Explained matches: %ld\n", matches);

    bsql_log_event("explain", "ok", "Explained search matches.");
    return 0;
}

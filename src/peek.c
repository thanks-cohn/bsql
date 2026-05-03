#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bsql.h"

static int contains_icase(const char *haystack, const char *needle) {
    if (!haystack || !needle) return 0;

    size_t nlen = strlen(needle);

    for (size_t i = 0; haystack[i]; i++) {
        size_t j = 0;
        while (j < nlen &&
               haystack[i + j] &&
               tolower((unsigned char)haystack[i + j]) ==
               tolower((unsigned char)needle[j])) {
            j++;
        }
        if (j == nlen) return 1;
    }
    return 0;
}

static void shorten_path(char *out, size_t size, const char *path) {
    const char *home = getenv("HOME");

    if (home && strstr(path, home) == path) {
        snprintf(out, size, "~%s", path + strlen(home));
    } else {
        snprintf(out, size, "%s", path);
    }
}

int bsql_peek(const char *query) {
    const char *home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "bsql peek error: HOME not set\n");
        return 1;
    }

    char index_path[4096];
    snprintf(index_path, sizeof(index_path),
        "%s/.local/share/bsql/index/bsql.index.jsonl", home);

    FILE *f = fopen(index_path, "r");
    if (!f) {
        fprintf(stderr, "bsql peek error: index not found\n");
        return 1;
    }

    printf("BetterSQL peek: %s\n\n", query);

    char line[8192];
    int shown = 0;
    int total = 0;
    int limit = 10;

    while (fgets(line, sizeof(line), f)) {
        if (!contains_icase(line, query)) continue;

        total++;

        if (shown >= limit) continue;

        // extract "path":"..."
        char *p = strstr(line, "\"path\":\"");
        if (!p) continue;

        p += 8;
        char *end = strchr(p, '"');
        if (!end) continue;

        char path[4096];
        size_t len = end - p;
        if (len >= sizeof(path)) len = sizeof(path) - 1;

        strncpy(path, p, len);
        path[len] = '\0';

        char pretty[4096];
        shorten_path(pretty, sizeof(pretty), path);

        printf("%d. %s\n", shown + 1, pretty);

        shown++;
    }

    fclose(f);

    printf("\nShowing %d of %d matches\n", shown, total);

    return 0;
}

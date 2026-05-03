#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "bsql.h"

static int file_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

static long count_lines(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;

    long lines = 0;
    int c;

    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') lines++;
    }

    fclose(f);
    return lines;
}

static long file_size(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return (long)st.st_size;
}

int bsql_status(void) {
    const char *home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "bsql status error: HOME not set\n");
        return 1;
    }

    char locations[4096];
    char index[4096];
    char log[4096];

    snprintf(locations, sizeof(locations), "%s/.config/bsql/locations.tsv", home);
    snprintf(index, sizeof(index), "%s/.local/share/bsql/index/bsql.index.jsonl", home);
    snprintf(log, sizeof(log), "%s/.local/share/bsql/logs/bsql.log", home);

    long location_count = count_lines(locations);
    long indexed_files = count_lines(index);

    printf("BetterSQL status\n\n");

    printf("Locations: %ld\n", location_count);
    printf("Index exists: %s\n", file_exists(index) ? "yes" : "no");
    printf("Indexed files: %ld\n", indexed_files);
    printf("Index size: %ld bytes\n", file_size(index));
    printf("Log exists: %s\n", file_exists(log) ? "yes" : "no");
    printf("Log size: %ld bytes\n", file_size(log));

    printf("\nPaths\n");
    printf("Locations: %s\n", locations);
    printf("Index:     %s\n", index);
    printf("Logs:      %s\n", log);

    bsql_log_event("status", "ok", "Printed BetterSQL status.");
    return 0;
}

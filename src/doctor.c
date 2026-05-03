#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
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

static long count_dead_locations(const char *locations_path) {
    FILE *f = fopen(locations_path, "r");
    if (!f) return 0;

    char line[4096];
    long dead = 0;

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = '\0';

        if (line[0] == '\0') continue;

        if (access(line, F_OK) != 0) {
            dead++;
        }
    }

    fclose(f);
    return dead;
}

static long count_missing_indexed_files(const char *index_path) {
    FILE *f = fopen(index_path, "r");
    if (!f) return 0;

    char line[16384];
    long missing = 0;

    while (fgets(line, sizeof(line), f)) {
        char path[4096] = "";

        if (!extract_json_string(line, "path", path, sizeof(path))) {
            continue;
        }

        if (access(path, F_OK) != 0) {
            missing++;
        }
    }

    fclose(f);
    return missing;
}

int bsql_doctor(void) {
    const char *home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "bsql doctor error: HOME not set\n");
        return 1;
    }

    char locations[4096];
    char index[4096];
    char log[4096];

    snprintf(locations, sizeof(locations), "%s/.config/bsql/locations.tsv", home);
    snprintf(index, sizeof(index), "%s/.local/share/bsql/index/bsql.index.jsonl", home);
    snprintf(log, sizeof(log), "%s/.local/share/bsql/logs/bsql.log", home);

    int issues = 0;

    long location_count = count_lines(locations);
    long indexed_files = count_lines(index);
    long dead_locations = count_dead_locations(locations);
    long missing_files = count_missing_indexed_files(index);

    printf("BetterSQL doctor\n\n");

    if (!file_exists(locations)) {
        printf("[warn] locations file missing: %s\n", locations);
        issues++;
    } else {
        printf("[ok] locations file exists: %s\n", locations);
        printf("     registered locations: %ld\n", location_count);
    }

    if (dead_locations > 0) {
        printf("[warn] dead registered locations: %ld\n", dead_locations);
        issues++;
    } else {
        printf("[ok] no dead registered locations detected\n");
    }

    if (!file_exists(index)) {
        printf("[warn] index missing: %s\n", index);
        issues++;
    } else {
        printf("[ok] index exists: %s\n", index);
        printf("     indexed records: %ld\n", indexed_files);
    }

    if (missing_files > 0) {
        printf("[warn] indexed files missing from disk: %ld\n", missing_files);
        issues++;
    } else {
        printf("[ok] no missing indexed files detected\n");
    }

    if (!file_exists(log)) {
        printf("[warn] log missing: %s\n", log);
        issues++;
    } else {
        printf("[ok] log exists: %s\n", log);
    }

    printf("\nIssues: %d\n", issues);

    if (issues == 0) {
        printf("Diagnosis: healthy\n");
        bsql_log_event("doctor", "ok", "Doctor found no issues.");
    } else {
        printf("Diagnosis: attention needed\n");
        bsql_log_event("doctor", "warn", "Doctor found issues.");
    }

    return 0;
}

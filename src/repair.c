#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bsql.h"

#define MAX_LOCATIONS 4096
#define PATH_SIZE 4096

static int already_seen(char seen[MAX_LOCATIONS][PATH_SIZE], int count, const char *path) {
    for (int i = 0; i < count; i++) {
        if (strcmp(seen[i], path) == 0) return 1;
    }
    return 0;
}

int bsql_repair(void) {
    const char *home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "bsql repair error: HOME not set\n");
        return 1;
    }

    char locations[PATH_SIZE];
    char tmp_path[PATH_SIZE];

    snprintf(locations, sizeof(locations), "%s/.config/bsql/locations.tsv", home);
    snprintf(tmp_path, sizeof(tmp_path), "%s/.config/bsql/locations.tsv.tmp", home);

    FILE *in = fopen(locations, "r");
    if (!in) {
        fprintf(stderr, "bsql repair: no locations file found: %s\n", locations);
        return 1;
    }

    FILE *out = fopen(tmp_path, "w");
    if (!out) {
        perror("bsql repair fopen tmp");
        fclose(in);
        return 1;
    }

    char (*seen)[PATH_SIZE] = calloc(MAX_LOCATIONS, PATH_SIZE);
    if (!seen) {
        fprintf(stderr, "bsql repair error: memory allocation failed\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    int seen_count = 0;

    char line[PATH_SIZE];
    long kept = 0;
    long removed_dead = 0;
    long removed_dupes = 0;
    long skipped_blank = 0;

    while (fgets(line, sizeof(line), in)) {
        line[strcspn(line, "\r\n")] = '\0';

        if (line[0] == '\0') {
            skipped_blank++;
            continue;
        }

        if (access(line, F_OK) != 0) {
            removed_dead++;
            continue;
        }

        if (already_seen(seen, seen_count, line)) {
            removed_dupes++;
            continue;
        }

        if (seen_count < MAX_LOCATIONS) {
            snprintf(seen[seen_count], PATH_SIZE, "%s", line);
            seen_count++;
        }

        fprintf(out, "%s\n", line);
        kept++;
    }

    fclose(in);
    fclose(out);
    free(seen);

    if (rename(tmp_path, locations) != 0) {
        perror("bsql repair rename");
        return 1;
    }

    printf("BetterSQL repair\n\n");
    printf("Repaired: %s\n", locations);
    printf("Kept locations: %ld\n", kept);
    printf("Removed dead locations: %ld\n", removed_dead);
    printf("Removed duplicate locations: %ld\n", removed_dupes);
    printf("Skipped blank lines: %ld\n", skipped_blank);
    printf("\nRepair V0: locations.tsv cleanup only.\n");
    printf("Real files, sidecars, and index are untouched.\n");

    bsql_log_event("repair", "ok", "Repaired locations file.");
    return 0;
}

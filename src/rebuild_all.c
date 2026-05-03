#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsql.h"

int bsql_rebuild_all(void) {
    const char *home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "bsql rebuild-all error: HOME not set\n");
        return 1;
    }

    // --- paths ---
    char index_path[4096];
    snprintf(index_path, sizeof(index_path),
        "%s/.local/share/bsql/index/bsql.index.jsonl", home);

    char locations_path[4096];
    snprintf(locations_path, sizeof(locations_path),
        "%s/.config/bsql/locations.tsv", home);

    // --- wipe index ONCE ---
    FILE *wipe = fopen(index_path, "w");
    if (!wipe) {
        fprintf(stderr, "bsql rebuild-all error: cannot reset index\n");
        return 1;
    }
    fclose(wipe);

    FILE *f = fopen(locations_path, "r");
    if (!f) {
        fprintf(stderr, "bsql rebuild-all error: locations file not found\n");
        return 1;
    }

    printf("BetterSQL rebuild-all\n\n");

    char line[4096];
    int compiled = 0;
    int skipped = 0;

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = '\0';

        if (line[0] == '\0') continue;

        // 🔥 skip test/junk dirs (critical)
        if (strncmp(line, "/tmp/", 5) == 0) {
            printf("[skip] %s (temp)\n", line);
            skipped++;
            continue;
        }

        printf("[scan] %s\n", line);

        int rc = bsql_scan(line);   // 👈 APPENDS (correct behavior)

        if (rc != 0) {
            fprintf(stderr, "warning: failed to scan %s\n", line);
            continue;
        }

        compiled++;
    }

    fclose(f);

    printf("\nDone.\n");
    printf("Compiled locations: %d\n", compiled);
    printf("Skipped locations:  %d\n", skipped);

    bsql_log_event("rebuild-all", "ok",
        "Rebuilt index from all valid locations.");

    return 0;
}
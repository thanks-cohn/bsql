#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include "bsql.h"

static void mkdir_if_missing(const char *path) {
    mkdir(path, 0755);
}

static void ensure_config_dirs(const char *home) {
    char path[4096];

    snprintf(path, sizeof(path), "%s/.config", home);
    mkdir_if_missing(path);

    snprintf(path, sizeof(path), "%s/.config/bsql", home);
    mkdir_if_missing(path);
}

int bsql_location(const char *path) {
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "bsql location error: HOME not set\n");
        return 1;
    }

    struct stat st;
    if (stat(path, &st) != 0 || !S_ISDIR(st.st_mode)) {
        fprintf(stderr, "bsql location error: not a readable directory: %s\n", path);
        return 1;
    }

    char resolved[PATH_MAX];
    if (!realpath(path, resolved)) {
        perror("bsql location realpath");
        return 1;
    }

    ensure_config_dirs(home);

    char config_path[4096];
    snprintf(config_path, sizeof(config_path), "%s/.config/bsql/locations.tsv", home);

    FILE *f = fopen(config_path, "a");
    if (!f) {
        perror("bsql location fopen");
        return 1;
    }

    fprintf(f, "%s\n", resolved);
    fclose(f);

    printf("Registered location: %s\n", resolved);

    bsql_log_event("location", "ok", "Registered searchable location.");
    return 0;
}

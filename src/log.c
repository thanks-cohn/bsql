#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "bsql.h"

static void mkdir_if_missing(const char *path) {
    mkdir(path, 0755);
}

static void ensure_log_dirs(const char *home) {
    char path[4096];

    snprintf(path, sizeof(path), "%s/.local", home);
    mkdir_if_missing(path);

    snprintf(path, sizeof(path), "%s/.local/share", home);
    mkdir_if_missing(path);

    snprintf(path, sizeof(path), "%s/.local/share/bsql", home);
    mkdir_if_missing(path);

    snprintf(path, sizeof(path), "%s/.local/share/bsql/logs", home);
    mkdir_if_missing(path);
}

int bsql_log_event(const char *command, const char *status, const char *summary) {
    const char *home = getenv("HOME");
    if (!home) return 1;

    ensure_log_dirs(home);

    char log_path[4096];
    snprintf(log_path, sizeof(log_path), "%s/.local/share/bsql/logs/bsql.log", home);

    FILE *f = fopen(log_path, "a");
    if (!f) return 1;

    time_t now = time(NULL);

    fprintf(
        f,
        "{\"ts\":%ld,\"tool\":\"bsql\",\"command\":\"%s\",\"status\":\"%s\",\"summary\":\"%s\"}\n",
        now,
        command,
        status,
        summary
    );

    fclose(f);
    return 0;
}

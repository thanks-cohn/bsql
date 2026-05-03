#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsql.h"

#define MAX_LINES 20
#define LINE_SIZE 4096

int bsql_logs(void) {
    const char *home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "bsql logs error: HOME not set\n");
        return 1;
    }

    char log_path[4096];
    snprintf(log_path, sizeof(log_path), "%s/.local/share/bsql/logs/bsql.log", home);

    FILE *f = fopen(log_path, "r");
    if (!f) {
        fprintf(stderr, "bsql logs error: log not found: %s\n", log_path);
        return 1;
    }

    char lines[MAX_LINES][LINE_SIZE];
    int count = 0;
    long total = 0;
    char buf[LINE_SIZE];

    while (fgets(buf, sizeof(buf), f)) {
        snprintf(lines[count % MAX_LINES], LINE_SIZE, "%s", buf);
        count++;
        total++;
    }

    fclose(f);

    int shown = count < MAX_LINES ? count : MAX_LINES;
    int start = count >= MAX_LINES ? count % MAX_LINES : 0;

    printf("BetterSQL logs\n\n");
    printf("Log: %s\n", log_path);
    printf("Showing last %d of %ld entries\n\n", shown, total);

    for (int i = 0; i < shown; i++) {
        int idx = (start + i) % MAX_LINES;
        printf("%s", lines[idx]);
    }

    return 0;
}

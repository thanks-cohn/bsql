#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "bsql.h"

static void json_escape(FILE *out, const char *s) {
    for (; *s; s++) {
        if (*s == '\\') fputs("\\\\", out);
        else if (*s == '"') fputs("\\\"", out);
        else if (*s == '\n') fputs("\\n", out);
        else if (*s == '\r') fputs("\\r", out);
        else if (*s == '\t') fputs("\\t", out);
        else fputc(*s, out);
    }
}

int bsql_summary(const char *path, const char *summary) {
    struct stat st;

    if (stat(path, &st) != 0) {
        fprintf(stderr, "bsql summary error: file not found: %s\n", path);
        return 1;
    }

    char sidecar[4096];
    snprintf(sidecar, sizeof(sidecar), "%s.wmeta.json", path);

    FILE *f = fopen(sidecar, "w");
    if (!f) {
        perror("bsql summary fopen");
        return 1;
    }

    fprintf(f, "{\n");
    fprintf(f, "  \"path\": \"");
    json_escape(f, path);
    fprintf(f, "\",\n");

    fprintf(f, "  \"summary\": \"");
    json_escape(f, summary);
    fprintf(f, "\"\n");

    fprintf(f, "}\n");
    fclose(f);

    printf("Summarized: %s\n", path);
    printf("Sidecar: %s\n", sidecar);
    printf("Summary: %s\n", summary);

    bsql_log_event("summary", "ok", "Wrote summary to sidecar.");
    return 0;
}

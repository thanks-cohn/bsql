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

static int file_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

int bsql_sidecar(const char *path) {
    if (!file_exists(path)) {
        fprintf(stderr, "bsql sidecar error: file not found: %s\n", path);
        return 1;
    }

    char sidecar[4096];
    snprintf(sidecar, sizeof(sidecar), "%s.wmeta.json", path);

    if (!file_exists(sidecar)) {
        FILE *f = fopen(sidecar, "w");
        if (!f) {
            perror("bsql sidecar fopen");
            return 1;
        }

        fprintf(f, "{\n");
        fprintf(f, "  \"path\": \"");
        json_escape(f, path);
        fprintf(f, "\",\n");
        fprintf(f, "  \"tags\": [],\n");
        fprintf(f, "  \"note\": \"\",\n");
        fprintf(f, "  \"summary\": \"\"\n");
        fprintf(f, "}\n");

        fclose(f);

        printf("Created sidecar: %s\n", sidecar);
    } else {
        printf("Sidecar exists: %s\n", sidecar);
    }

    FILE *f = fopen(sidecar, "r");
    if (!f) {
        perror("bsql sidecar read");
        return 1;
    }

    printf("\n");

    int c;
    while ((c = fgetc(f)) != EOF) {
        fputc(c, stdout);
    }

    fclose(f);

    bsql_log_event("sidecar", "ok", "Created or printed sidecar.");
    return 0;
}

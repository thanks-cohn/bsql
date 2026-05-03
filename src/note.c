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

int bsql_note(const char *path, const char *note) {
    struct stat st;

    if (stat(path, &st) != 0) {
        fprintf(stderr, "bsql note error: file not found: %s\n", path);
        return 1;
    }

    char sidecar[4096];
    snprintf(sidecar, sizeof(sidecar), "%s.wmeta.json", path);

    FILE *f = fopen(sidecar, "w");
    if (!f) {
        perror("bsql note fopen");
        return 1;
    }

    fprintf(f, "{\n");
    fprintf(f, "  \"path\": \"");
    json_escape(f, path);
    fprintf(f, "\",\n");

    fprintf(f, "  \"note\": \"");
    json_escape(f, note);
    fprintf(f, "\"\n");

    fprintf(f, "}\n");
    fclose(f);

    printf("Noted: %s\n", path);
    printf("Sidecar: %s\n", sidecar);
    printf("Note: %s\n", note);

    bsql_log_event("note", "ok", "Wrote note to sidecar.");
    return 0;
}

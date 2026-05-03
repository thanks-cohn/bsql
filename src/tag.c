#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int bsql_tag(const char *path, int tag_count, char **tags) {
    struct stat st;

    if (stat(path, &st) != 0) {
        fprintf(stderr, "bsql tag error: file not found: %s\n", path);
        return 1;
    }

    char sidecar[4096];
    snprintf(sidecar, sizeof(sidecar), "%s.wmeta.json", path);

    FILE *f = fopen(sidecar, "w");
    if (!f) {
        perror("bsql tag fopen");
        return 1;
    }

    fprintf(f, "{\n");
    fprintf(f, "  \"path\": \"");
    json_escape(f, path);
    fprintf(f, "\",\n");

    fprintf(f, "  \"tags\": [");
    for (int i = 0; i < tag_count; i++) {
        fprintf(f, "\"");
        json_escape(f, tags[i]);
        fprintf(f, "\"");
        if (i + 1 < tag_count) fprintf(f, ", ");
    }
    fprintf(f, "]\n");

    fprintf(f, "}\n");
    fclose(f);

    printf("Tagged: %s\n", path);
    printf("Sidecar: %s\n", sidecar);
    printf("Tags:");
    for (int i = 0; i < tag_count; i++) {
        printf(" %s", tags[i]);
    }
    printf("\n");

    bsql_log_event("tag", "ok", "Wrote tags to sidecar.");
    return 0;
}

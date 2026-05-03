#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include "bsql.h"

static void mkdir_if_missing(const char *path) {
    mkdir(path, 0755);
}

static void ensure_index_dirs(const char *home) {
    char path[4096];

    snprintf(path, sizeof(path), "%s/.local", home);
    mkdir_if_missing(path);

    snprintf(path, sizeof(path), "%s/.local/share", home);
    mkdir_if_missing(path);

    snprintf(path, sizeof(path), "%s/.local/share/bsql", home);
    mkdir_if_missing(path);

    snprintf(path, sizeof(path), "%s/.local/share/bsql/index", home);
    mkdir_if_missing(path);
}

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

static const char *base_name(const char *path) {
    const char *slash = strrchr(path, '/');
    return slash ? slash + 1 : path;
}

static const char *kind_from_path(const char *path) {
    const char *dot = strrchr(path, '.');
    if (!dot || dot == path) return "unknown";

    if (strcmp(dot, ".png") == 0) return "image";
    if (strcmp(dot, ".jpg") == 0) return "image";
    if (strcmp(dot, ".jpeg") == 0) return "image";
    if (strcmp(dot, ".webp") == 0) return "image";
    if (strcmp(dot, ".gif") == 0) return "image";
    if (strcmp(dot, ".ppm") == 0) return "image";
    if (strcmp(dot, ".pdf") == 0) return "pdf";
    if (strcmp(dot, ".txt") == 0) return "text";
    if (strcmp(dot, ".md") == 0) return "text";
    if (strcmp(dot, ".json") == 0) return "json";
    if (strcmp(dot, ".jsonl") == 0) return "jsonl";
    if (strcmp(dot, ".c") == 0) return "code";
    if (strcmp(dot, ".h") == 0) return "code";
    if (strcmp(dot, ".py") == 0) return "code";
    if (strcmp(dot, ".sh") == 0) return "code";

    return "file";
}

static int should_skip(const char *path) {
    if (strstr(path, "/.git/")) return 1;
    if (strstr(path, "/build/")) return 1;
    if (strstr(path, "/node_modules/")) return 1;
    if (strstr(path, "/.cache/")) return 1;
    if (strstr(path, ".wmeta.json")) return 1;
    return 0;
}

static int read_file(const char *path, char *buf, size_t n) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;

    size_t got = fread(buf, 1, n - 1, f);
    buf[got] = '\0';
    fclose(f);
    return 1;
}

static int extract_json_string(const char *json, const char *key, char *out, size_t n) {
    char pattern[128];
    snprintf(pattern, sizeof(pattern), "\"%s\": \"", key);

    const char *p = strstr(json, pattern);
    if (!p) {
        snprintf(pattern, sizeof(pattern), "\"%s\":\"", key);
        p = strstr(json, pattern);
    }

    if (!p) return 0;

    p = strchr(p, ':');
    if (!p) return 0;

    p++;
    while (*p == ' ') p++;
    if (*p != '"') return 0;
    p++;

    size_t i = 0;
    while (*p && *p != '"' && i + 1 < n) {
        out[i++] = *p++;
    }

    out[i] = '\0';
    return 1;
}

static int extract_tags_raw(const char *json, char *out, size_t n) {
    const char *p = strstr(json, "\"tags\"");
    if (!p) return 0;

    p = strchr(p, '[');
    if (!p) return 0;

    const char *e = strchr(p, ']');
    if (!e) return 0;

    size_t len = (size_t)(e - p + 1);
    if (len >= n) len = n - 1;

    memcpy(out, p, len);
    out[len] = '\0';
    return 1;
}

static void write_record(FILE *index, const char *path, struct stat *st) {
    char sidecar[8192];
    char sidecar_json[32768];
    char note[4096] = "";
    char summary[4096] = "";
    char tags[8192] = "[]";

    snprintf(sidecar, sizeof(sidecar), "%s.wmeta.json", path);

    if (read_file(sidecar, sidecar_json, sizeof(sidecar_json))) {
        extract_json_string(sidecar_json, "note", note, sizeof(note));
        extract_json_string(sidecar_json, "summary", summary, sizeof(summary));
        extract_tags_raw(sidecar_json, tags, sizeof(tags));
    }

    fprintf(index, "{");

    fprintf(index, "\"path\":\"");
    json_escape(index, path);
    fprintf(index, "\",");

    fprintf(index, "\"name\":\"");
    json_escape(index, base_name(path));
    fprintf(index, "\",");

    fprintf(index, "\"kind\":\"%s\",", kind_from_path(path));
    fprintf(index, "\"size\":%ld,", (long)st->st_size);
    fprintf(index, "\"mtime\":%ld,", (long)st->st_mtime);

    fprintf(index, "\"tags\":%s,", tags);

    fprintf(index, "\"note\":\"");
    json_escape(index, note);
    fprintf(index, "\",");

    fprintf(index, "\"summary\":\"");
    json_escape(index, summary);
    fprintf(index, "\"");

    fprintf(index, "}\n");
}

static int compile_walk(const char *path, FILE *index, long *count) {
    DIR *dir = opendir(path);
    if (!dir) return 0;

    struct dirent *ent;

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;

        char child[PATH_MAX];
        snprintf(child, sizeof(child), "%s/%s", path, ent->d_name);

        if (should_skip(child)) continue;

        struct stat st;
        if (lstat(child, &st) != 0) continue;

        if (S_ISDIR(st.st_mode)) {
            compile_walk(child, index, count);
        } else if (S_ISREG(st.st_mode)) {
            char resolved[PATH_MAX];
            const char *real = realpath(child, resolved) ? resolved : child;
            write_record(index, real, &st);
            (*count)++;
        }
    }

    closedir(dir);
    return 0;
}

int bsql_compile(const char *path) {
    const char *home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "bsql compile error: HOME not set\n");
        return 1;
    }

    struct stat st;
    if (stat(path, &st) != 0 || !S_ISDIR(st.st_mode)) {
        fprintf(stderr, "bsql compile error: not a readable directory: %s\n", path);
        return 1;
    }

    ensure_index_dirs(home);

    char index_path[4096];
    snprintf(index_path, sizeof(index_path), "%s/.local/share/bsql/index/bsql.index.jsonl", home);

    FILE *index = fopen(index_path, "w");
    if (!index) {
        perror("bsql compile fopen");
        return 1;
    }

    long count = 0;
    compile_walk(path, index, &count);
    fclose(index);

    printf("Compiled: %s\n", path);
    printf("Files indexed: %ld\n", count);
    printf("Index: %s\n", index_path);

    bsql_log_event("compile", "ok", "Compiled files and sidecars into JSONL index.");
    return 0;
}

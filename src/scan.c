#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
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

static const char *kind_from_path(const char *path) {
    const char *dot = strrchr(path, '.');
    if (!dot || dot == path) return "unknown";

    if (strcmp(dot, ".png") == 0) return "image";
    if (strcmp(dot, ".jpg") == 0) return "image";
    if (strcmp(dot, ".jpeg") == 0) return "image";
    if (strcmp(dot, ".webp") == 0) return "image";
    if (strcmp(dot, ".gif") == 0) return "image";
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

static const char *base_name(const char *path) {
    const char *slash = strrchr(path, '/');
    return slash ? slash + 1 : path;
}

static int should_skip(const char *path) {
    if (strstr(path, "/.git/")) return 1;
    if (strstr(path, "/build/")) return 1;
    if (strstr(path, "/node_modules/")) return 1;
    if (strstr(path, "/.cache/")) return 1;
    return 0;
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

static int scan_walk(const char *path, FILE *index, long *count) {
    DIR *dir = opendir(path);
    if (!dir) return 0;

    struct dirent *ent;

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        char child[PATH_MAX];
        snprintf(child, sizeof(child), "%s/%s", path, ent->d_name);

        if (should_skip(child)) continue;

        struct stat st;
        if (lstat(child, &st) != 0) continue;

        if (S_ISDIR(st.st_mode)) {
            scan_walk(child, index, count);
        } else if (S_ISREG(st.st_mode)) {
            char resolved[PATH_MAX];
            const char *real = realpath(child, resolved) ? resolved : child;

            fprintf(index, "{");
            fprintf(index, "\"path\":\"");
            json_escape(index, real);
            fprintf(index, "\",\"name\":\"");
            json_escape(index, base_name(real));
            fprintf(index, "\",\"kind\":\"%s\"", kind_from_path(real));
            fprintf(index, ",\"size\":%ld", (long)st.st_size);
            fprintf(index, ",\"mtime\":%ld", (long)st.st_mtime);
            fprintf(index, "}\n");

            (*count)++;
        }
    }

    closedir(dir);
    return 0;
}

int bsql_scan(const char *path) {
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "bsql scan error: HOME not set\n");
        return 1;
    }

    struct stat st;
    if (stat(path, &st) != 0 || !S_ISDIR(st.st_mode)) {
        fprintf(stderr, "bsql scan error: not a readable directory: %s\n", path);
        return 1;
    }

    ensure_index_dirs(home);

    char index_path[4096];
    snprintf(index_path, sizeof(index_path), "%s/.local/share/bsql/index/bsql.index.jsonl", home);

    FILE *index = fopen(index_path, "a");
    if (!index) {
        perror("bsql scan fopen");
        return 1;
    }

    long count = 0;
    scan_walk(path, index, &count);
    fclose(index);

    printf("Scanned: %s\n", path);
    printf("Files indexed: %ld\n", count);
    printf("Index: %s\n", index_path);

    bsql_log_event("scan", "ok", "Scanned folder and rebuilt JSONL index.");
    return 0;
}

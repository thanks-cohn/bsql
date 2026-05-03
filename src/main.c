#include <stdio.h>
#include <string.h>
#include "bsql.h"

static void help(void) {
    printf("BetterSQL\n\n");
    printf("Usage:\n");
    printf("  bsql where [--json]\n");
    printf("  bsql status\n");
printf("  bsql doctor\n");
printf("  bsql clean\n");
printf("  bsql logs\n");
printf("  bsql missing\n");
printf("  bsql history <path>\n");
printf("  bsql id <path>\n");
    printf("  bsql location <path>\n");
    printf("  bsql scan <path>\n");
    printf("  bsql compile <path>\n");
printf("  bsql rebuild <path>\n");
    printf("  bsql search <query>\n");
    printf("  bsql find <query>\n");
    printf("  bsql meta <path>\n");
    printf("  bsql explain <query>\n");
    printf("  bsql tag <path> <tag> [tag...]\n");
    printf("  bsql note <path> <note>\n");
    printf("  bsql summary <path> <summary>\n");
    printf("  bsql sidecar <path>\n");
    printf("  bsql help\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        help();
        return 0;
    }

    if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "--help") == 0) {
        help();
        return 0;
    }

    if (strcmp(argv[1], "where") == 0) {
        int json = 0;
        if (argc >= 3 && strcmp(argv[2], "--json") == 0) json = 1;
        return bsql_where(json);
    }

    if (strcmp(argv[1], "status") == 0) {
        return bsql_status();
    }

    if (strcmp(argv[1], "doctor") == 0) {
        return bsql_doctor();
    }

    if (strcmp(argv[1], "clean") == 0) {
        return bsql_clean();
    }

    if (strcmp(argv[1], "logs") == 0) {
        return bsql_logs();
    }

    if (strcmp(argv[1], "missing") == 0) {
        return bsql_missing();
    }

    if (strcmp(argv[1], "history") == 0) {
        if (argc < 3) {
            fprintf(stderr, "bsql history error: missing path\n");
            return 1;
        }
        return bsql_history(argv[2]);
    }

    if (strcmp(argv[1], "id") == 0) {
        if (argc < 3) {
            fprintf(stderr, "bsql id error: missing path\n");
            return 1;
        }
        return bsql_id(argv[2]);
    }

    if (strcmp(argv[1], "location") == 0) {
        if (argc < 3) {
            fprintf(stderr, "bsql location error: missing path\n");
            return 1;
        }
        return bsql_location(argv[2]);
    }

    if (strcmp(argv[1], "scan") == 0) {
        if (argc < 3) {
            fprintf(stderr, "bsql scan error: missing path\n");
            return 1;
        }
        return bsql_scan(argv[2]);
    }

    if (strcmp(argv[1], "compile") == 0) {
        if (argc < 3) {
            fprintf(stderr, "bsql compile error: missing path\n");
            return 1;
        }
        return bsql_compile(argv[2]);
    }

    if (strcmp(argv[1], "rebuild") == 0) {
        if (argc < 3) {
            fprintf(stderr, "bsql rebuild error: missing path\n");
            return 1;
        }
        return bsql_rebuild(argv[2]);
    }

    if (strcmp(argv[1], "search") == 0 || strcmp(argv[1], "find") == 0) {
        if (argc < 3) {
            fprintf(stderr, "bsql search error: missing query\n");
            return 1;
        }
        return bsql_search(argv[2]);
    }

    if (strcmp(argv[1], "meta") == 0) {
        if (argc < 3) {
            fprintf(stderr, "bsql meta error: missing path\n");
            return 1;
        }
        return bsql_meta(argv[2]);
    }

    if (strcmp(argv[1], "explain") == 0) {
        if (argc < 3) {
            fprintf(stderr, "bsql explain error: missing query\n");
            return 1;
        }
        return bsql_explain(argv[2]);
    }

    if (strcmp(argv[1], "tag") == 0) {
        if (argc < 4) {
            fprintf(stderr, "bsql tag error: usage: bsql tag <path> <tag> [tag...]\n");
            return 1;
        }
        return bsql_tag(argv[2], argc - 3, &argv[3]);
    }

    if (strcmp(argv[1], "note") == 0) {
        if (argc < 4) {
            fprintf(stderr, "bsql note error: usage: bsql note <path> <note>\n");
            return 1;
        }
        return bsql_note(argv[2], argv[3]);
    }

    if (strcmp(argv[1], "summary") == 0) {
        if (argc < 4) {
            fprintf(stderr, "bsql summary error: usage: bsql summary <path> <summary>\n");
            return 1;
        }
        return bsql_summary(argv[2], argv[3]);
    }

    if (strcmp(argv[1], "sidecar") == 0) {
        if (argc < 3) {
            fprintf(stderr, "bsql sidecar error: missing path\n");
            return 1;
        }
        return bsql_sidecar(argv[2]);
    }

    fprintf(stderr, "bsql error: unknown command: %s\n", argv[1]);
    return 1;
}

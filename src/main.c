#include <stdio.h>
#include <string.h>
#include "bsql.h"

static void help(void) {
    printf("BetterSQL\n\n");
    printf("Usage:\n");
    printf("  bsql where [--json]\n");
    printf("  bsql location <path>\n");
    printf("  bsql scan <path>\n");
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
        if (argc >= 3 && strcmp(argv[2], "--json") == 0) {
            json = 1;
        }
        return bsql_where(json);
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

    fprintf(stderr, "bsql error: unknown command: %s\n", argv[1]);
    return 1;
}

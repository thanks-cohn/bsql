#include <stdio.h>
#include <stdlib.h>
#include "bsql.h"

int bsql_user(void) {
    const char *home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "bsql user error: HOME not set\n");
        return 1;
    }

    printf("BetterSQL user\n\n");
    printf("Indexing home directory: %s\n\n", home);

    int rc = bsql_rebuild(home);

    if (rc == 0) {
        printf("\nHome search index ready.\n");
        printf("Try:\n");
        printf("  bsql search screenshot\n");
        printf("  bsql search pdf\n");
        printf("  bsql search invoice\n");
        bsql_log_event("user", "ok", "Indexed user home directory.");
    } else {
        bsql_log_event("user", "error", "Failed to index user home directory.");
    }

    return rc;
}

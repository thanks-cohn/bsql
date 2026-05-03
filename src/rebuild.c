#include <stdio.h>
#include "bsql.h"

int bsql_rebuild(const char *path) {
    printf("BetterSQL rebuild\n\n");
    printf("Rebuilding index from files and sidecars...\n\n");

    int rc = bsql_compile(path);

    if (rc == 0) {
        printf("\nRebuild complete.\n");
        bsql_log_event("rebuild", "ok", "Rebuilt index from files and sidecars.");
    } else {
        bsql_log_event("rebuild", "error", "Failed to rebuild index.");
    }

    return rc;
}

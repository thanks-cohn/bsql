#include <stdio.h>
#include <stdlib.h>
#include "bsql.h"

int bsql_user(void) {
    const char *home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "bsql user error: HOME not set\n");
        return 1;
    }

    const char *dirs[] = {
        "Documents",
        "Pictures",
        "Videos",
        "Desktop",
        "Downloads"
    };

    printf("BetterSQL user (focused index)\n\n");

    for (int i = 0; i < 5; i++) {
        char path[4096];
        snprintf(path, sizeof(path), "%s/%s", home, dirs[i]);

        printf("[scan] %s\n", path);
        bsql_rebuild(path);
    }

    printf("\nUser-focused index complete.\n");

    return 0;
}

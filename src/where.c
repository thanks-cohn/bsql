#include <stdio.h>
#include <stdlib.h>
#include "bsql.h"

static const char *bsql_home(void) {
    const char *h = getenv("HOME");
    return h ? h : "";
}

int bsql_where(int json) {
    const char *h = bsql_home();

    if (json) {
        printf("{\n");
        printf("  \"tool\": \"bsql\",\n");
        printf("  \"command\": \"where\",\n");
        printf("  \"config_path\": \"%s/.config/bsql/config.json\",\n", h);
        printf("  \"data_path\": \"%s/.local/share/bsql\",\n", h);
        printf("  \"index_path\": \"%s/.local/share/bsql/index/bsql.index.jsonl\",\n", h);
        printf("  \"log_path\": \"%s/.local/share/bsql/logs/bsql.log\"\n", h);
        printf("}\n");
    } else {
        printf("BetterSQL paths\n\n");
        printf("Config: %s/.config/bsql/config.json\n", h);
        printf("Data:   %s/.local/share/bsql\n", h);
        printf("Index:  %s/.local/share/bsql/index/bsql.index.jsonl\n", h);
        printf("Logs:   %s/.local/share/bsql/logs/bsql.log\n", h);
    }

    bsql_log_event("where", "ok", "Printed BetterSQL paths.");
    return 0;
}

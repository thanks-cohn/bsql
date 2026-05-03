#ifndef BSQL_H
#define BSQL_H

int bsql_where(int json);
int bsql_log_event(const char *command, const char *status, const char *summary);
int bsql_location(const char *path);
int bsql_scan(const char *path);
int bsql_search(const char *query);
int bsql_meta(const char *path);
int bsql_explain(const char *query);
int bsql_tag(const char *path, int tag_count, char **tags);
int bsql_note(const char *path, const char *note);
int bsql_summary(const char *path, const char *summary);
int bsql_sidecar(const char *path);
int bsql_compile(const char *path);
int bsql_status(void);
int bsql_clean(void);
int bsql_logs(void);
int bsql_missing(void);
int bsql_rebuild(const char *path);
int bsql_doctor(void);
int bsql_history(const char *path);
int bsql_id(const char *path);
int bsql_context(const char *path);
int bsql_repair(void);
int bsql_user(void);

#endif

int bsql_rebuild_all(void);

int bsql_peek(const char *query);

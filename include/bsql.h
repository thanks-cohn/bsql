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

#endif

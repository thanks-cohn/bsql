#ifndef BSQL_H
#define BSQL_H

int bsql_where(int json);
int bsql_log_event(const char *command, const char *status, const char *summary);
int bsql_location(const char *path);

#endif

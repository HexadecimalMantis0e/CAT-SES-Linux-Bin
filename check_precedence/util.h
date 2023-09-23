#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <sqlext.h>

void ses_log(const char *logType, const char *logMsg);
void cleanup(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt);

#endif
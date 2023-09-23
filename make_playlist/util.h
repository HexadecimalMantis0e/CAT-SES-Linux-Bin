#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sqlext.h>

void get_time();
void cleanup(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt, FILE *fp);

#endif
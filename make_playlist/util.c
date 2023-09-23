#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlext.h>
#include "shared.h"
#include "util.h"

void get_time() {
    time(&rawtime);
    ptr_tm = gmtime(&rawtime);
    memcpy(&utc_tm, ptr_tm, sizeof(struct tm));
    ptr_tm = localtime(&rawtime);
    memcpy(&local_tm, ptr_tm, sizeof(struct tm));
}

void cleanup(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt, FILE *fp) {
    SQLFreeStmt(hstmt, SQL_CLOSE);
    SQLDisconnect(hdbc);
    SQLFreeConnect(hdbc);
    SQLFreeEnv(henv);
    fclose(fp);
}
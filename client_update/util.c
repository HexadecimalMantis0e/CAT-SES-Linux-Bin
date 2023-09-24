#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlext.h>

void ses_log(const char *logType, const char *logMsg) {
	char dest[520];
	memcpy(dest, "/SES/bin/ses_log ", 0x12);
    strcat(dest, logType);
    strcat(dest, " ");
    strcat(dest, "\"");
    strcat(dest, logMsg);
    strcat(dest, "\"");
    system(dest);
}

void cleanup(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt) {
    SQLFreeStmt(hstmt, SQL_CLOSE);
    SQLDisconnect(hdbc);
    SQLFreeConnect(hdbc);
    SQLFreeEnv(henv);
}
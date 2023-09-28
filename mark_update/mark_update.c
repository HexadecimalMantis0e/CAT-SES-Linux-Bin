#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlext.h>
#include "util.h"

int main(int argc, const char *argv[], const char *envp[]) {
    if (argc != 2) {
        ses_log("SES_ERR", "mark_update called without update filename.");
        exit(1);
    }

    int nameLength = strlen(argv[1]);

    if (nameLength <= 0) {
        ses_log("SES_ERR", "mark_update: Missing update filename.");
        exit(1);
    }

    char dest[128];
    strcpy(dest, argv[1]);

    char s[128];
    sprintf(s, "Mark client update %s as completed", dest);
    ses_log("SES_SYS", s);

    SQLHENV henv;
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER *) SQL_OV_ODBC3, 0);

    SQLHDBC hdbc;
    SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    SQLDriverConnect(hdbc, NULL, (SQLCHAR *) "DSN=client_db;", SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

    SQLHSTMT hstmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    char selectStmt[256];
    memcpy(selectStmt, "SELECT title_id FROM client_updates WHERE filename='", 0x35);
    strcat(selectStmt, dest);
    strcat(selectStmt, "'");

    SQLRETURN ret = SQLExecDirect(hstmt, (SQLCHAR *) selectStmt, SQL_NTS);

    if (ret) {
        sprintf(s, "Problem with SQL statement in mark_update (ret=%d)\n", (int) ret);
        ses_log("SES_ERR", s);
        cleanup(henv, hdbc, hstmt);
        exit(1);
    }

    char src[24];
    ret = SQLBindCol(hstmt, 1, SQL_C_CHAR, src, 17, NULL);

    if (ret) {
        sprintf(s, "mark_update: Error in SQL: Unable to bind title ID column (ret=%d)\n", (int) ret);
        ses_log("SES_ERR", s);
        cleanup(henv, hdbc, hstmt);
        exit(1);
    }

    ret = SQLFetch(hstmt);
    char fetchData[32];

    if (ret == SQL_NO_DATA) {
        memcpy(fetchData, "\0", 0x01);
    }
    else {
        strcpy(fetchData, src);
    }

    SQLFreeStmt(hstmt, SQL_CLOSE);
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    char updateStmt[256];
    memcpy(updateStmt, "UPDATE client_updates SET install_success=1 WHERE title_id=", 0x3C);
    strcat(updateStmt, "'");
    strcat(updateStmt, fetchData);
    strcat(updateStmt, "'");
    ret = SQLExecDirect(hstmt, (SQLCHAR *) updateStmt, SQL_NTS);

    if (ret) {
        sprintf(s, "Problem with SQL statement in mark_update (ret=%d)\n", (int) ret);
        ses_log("SES_ERR", s);
        cleanup(henv, hdbc, hstmt);
        exit(1);
    }

    cleanup(henv, hdbc, hstmt);
    exit(0);
}
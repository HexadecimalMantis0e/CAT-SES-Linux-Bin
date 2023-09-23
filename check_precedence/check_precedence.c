#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlext.h>
#include "util.h"

int main(int argc, const char *argv[], const char *envp[]) {
    ses_log("SES_SYS", "Check for required precedent");

    if (argc != 2) {
        ses_log("SES_ERR", "check_precedence called without title ID.");
        exit(1);
    }

    int idLength = strlen(argv[1]);

    if (idLength != 16) {
        ses_log("SES_ERR", "check_precedence: Title ID supplied is invalid length.");
        exit(1);
    }

    char dest[24];
    strcpy(dest, argv[1]);

    SQLHENV henv;
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER *)SQL_OV_ODBC3, 0);

    SQLHDBC hdbc;
    SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    SQLDriverConnect(hdbc, NULL, (SQLCHAR *) "DSN=client_db;", SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

    SQLHSTMT hstmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    char selectStmt[256];
    memcpy(selectStmt, "SELECT idclient_updates FROM client_updates WHERE title_id=", 0x3C);
    strcat(selectStmt, "'");
    strcat(selectStmt, dest);
    strcat(selectStmt, "' AND `install_success`=1");

    SQLRETURN ret = SQLExecDirect(hstmt, (SQLCHAR *) selectStmt, SQL_NTS);

    char s[128];

    if (ret) {
        sprintf(s, "Problem with SQL statement in check_precedence (ret=%d)\n", (int) ret);
        ses_log("SES_ERR", s);
        cleanup(henv, hdbc, hstmt);
        exit(1);
    }

    char src[6];
    ret = SQLBindCol(hstmt, 1, SQL_C_SLONG, src, 4, NULL);

    if (ret) {
        sprintf(s, "usb_update: Error in SQL: Unable to bind update ID column (ret=%d)\n", (int) ret);
        ses_log("SES_ERR", s);
        cleanup(henv, hdbc, hstmt);
        exit(1);
    }

    ret = SQLFetch(hstmt);

    if (ret != SQL_NO_DATA) {
        memcpy(s, "client_update precedent title id, ", 0x23);
        strcat(s, dest);
        strcat(s, " has been applied. Proceed...");
        ses_log("SES_SYS", s);
        cleanup(henv, hdbc, hstmt);
        exit(0);
    }

    memcpy(s, "client_update precedent title id, ", 0x23);
    strcat(s, dest);
    strcat(s, " was not found. Cannot install update.");
    ses_log("SES_SYS", s);
    cleanup(henv, hdbc, hstmt);
    exit(1);
}
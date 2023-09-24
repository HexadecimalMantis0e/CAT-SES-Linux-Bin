#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlext.h>
#include "util.h"

int main(int argc, const char *argv[], const char *envp[]) {
    ses_log("SES_SYS", "Started client update application");

    SQLHENV henv;
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER *) SQL_OV_ODBC3, 0);

    SQLHDBC hdbc;
    SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    SQLDriverConnect(hdbc, NULL, (SQLCHAR *) "DSN=client_db;", SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

    SQLHSTMT hstmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    int count = 0;

    char dest[256];
    memcpy(dest, "SELECT title_id FROM client_updates WHERE install_success=1", 0x3C);
    SQLRETURN ret = SQLExecDirect(hstmt, (SQLCHAR *) dest, SQL_NTS);

    char s[128];

    if (ret) {
        sprintf(s, "Problem with SQL statement in client_update (ret=%d)\n", (int) ret);
        ses_log("SES_ERR", s);
        cleanup(henv, hdbc, hstmt);
        exit(1);
    }

    char src[24];
    ret = SQLBindCol(hstmt, 1, SQL_C_CHAR, src, 17, NULL);

    if (ret) {
        sprintf(s, "client_update: Error in SQL: Unable to bind title ID column (ret=%d)\n", (int) ret);
        ses_log("SES_ERR", s);
        cleanup(henv, hdbc, hstmt);
        exit(1);
    }

    char *updates[2500];

    for (ret = SQLFetch(hstmt); ret != SQL_NO_DATA; ret = SQLFetch(hstmt)) {
        int index = count;
        updates[index] = (char *) malloc(0x11);
        strcpy(updates[count++], src);

        if (count > 2499) {
            sprintf(s, "client_update: Exceeded hard-coded maximum updates(%d)\n", 2500);
            ses_log("SES_ERR", s);
            cleanup(henv, hdbc, hstmt);
            exit(1);
        }
    }

    SQLFreeStmt(hstmt, SQL_CLOSE);
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    for (int i = 0; i < count; i++) {
        char updateStmt[256];
        sprintf(updateStmt, "UPDATE client_updates SET install_success=1 WHERE title_id='%s'", updates[i]);
        ret = SQLExecDirect(hstmt, (SQLCHAR *) updateStmt, SQL_NTS);

        if (ret) {
            sprintf(s, "client update: Failed trying to mark duplicate as already installed (ret=%d)\n", (int) ret);
            ses_log("SES_ERR", s);
        }

        SQLFreeStmt(hstmt, SQL_CLOSE);
        SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    }

    for (int i = 0; i < count; i++) {
        free(updates[i]);
    }

    memcpy(dest, "SELECT `idclient_updates`,`filename` FROM client_updates WHERE install_success=0 AND deleted=0", 0x5F);
    ret = SQLExecDirect(hstmt, (SQLCHAR *) dest, SQL_NTS);

    if (ret) {
        sprintf(s, "Problem with SQL statement in client_update (ret=%d)\n", (int) ret);
        ses_log("SES_ERR", s);
        cleanup(henv, hdbc, hstmt);
        exit(1);
    }

    char updateID[6];
    ret = SQLBindCol(hstmt, 1, SQL_C_SLONG, updateID, 4, NULL);

    if (ret) {
        sprintf(s, "client_update: Error in SQL: Unable to bind update ID column (ret=%d)\n", (int) ret);
        ses_log("SES_ERR", s);
        cleanup(henv, hdbc, hstmt);
        exit(1);
    }

    char updateName[128];
    ret = SQLBindCol(hstmt, 2, SQL_C_CHAR, updateName, 128, NULL);

    if (ret) {
        sprintf(s, "client_update: Error in SQL: Unable to bind update ID column (ret=%d)\n", (int) ret);
        ses_log("SES_ERR", s);
        cleanup(henv, hdbc, hstmt);
        exit(1);
    }

    ret = SQLFetch(hstmt);

    while (ret != SQL_NO_DATA) {
        memcpy(s, "Apply client update ", 0x15);
        strcat(s, updateName);
        strcat(s, " after unpacking.");
        ses_log("SES_SYS", s);

        char command[512];
        memcpy(command, "/SES/scripts/untar_update_noencrypt.sh ", 0x28);
        strcat(command, updateName);
        system(command);
        ret = SQLFetch(hstmt);
        sleep(2);
    }

    cleanup(henv, hdbc, hstmt);
    ses_log("SES_SYS", "Finished finding/applying client updates");
    exit(0);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlext.h>
#include "util.h"
#include "shared.h"

int main(int argc, const char *argv[], const char *envp[]) {
    int dummy = 0;
    char dest[128];
    memcpy(dest, "/SES/contents/videos/", 0x16);
    strcat(dest, "ses_playlist.txt");

    FILE *s = fopen(dest, "w");

    if (!s) {
        fprintf(stderr, "Error opening playlist:%s\n", dest);
        exit(1);
    }

    fwrite("[playlist]\n", 1, 0x0B, s);

    SQLHENV henv;
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER *) SQL_OV_ODBC3, 0);

    SQLHDBC hdbc;
    SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    SQLDriverConnect(hdbc, NULL, (SQLCHAR *) "DSN=client_db;", SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

    SQLHSTMT hstmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    get_time();
    t_convert = localtime(&rawtime);

    char src[128];

    if (!strftime(src, 0x80, "%Y%m%d%H%M%S", t_convert)) {
        fwrite("\nError converting time(strftime)", 1, 0x20, stderr);
        cleanup(henv, hdbc, hstmt, s);
        exit(1);
    }

    char selectStmt[256];
    memcpy(selectStmt, "SELECT `filename`,`idpromo_videos` FROM `promo_videos` WHERE `verified_on_disk`=1 AND `deleted`=0 AND `start_date`<\"", 0x75);

    strcat(selectStmt, src);
    strcat(selectStmt, "\" AND `end_date`>\"");
    strcat(selectStmt, src);
    strcat(selectStmt, "\"");

    SQLRETURN ret = SQLExecDirect(hstmt, (SQLCHAR *) selectStmt, SQL_NTS);

    if (ret) {
        fprintf(stderr, "Unable to execute statement directly (ret=%d)\n", (int) ret);
        cleanup(henv, hdbc, hstmt, s);
        exit(1);
    }

    ret = SQLBindCol(hstmt, 1, SQL_C_CHAR, dest, 128, NULL);

    if (ret) {
        fprintf(stderr, "Unable to bind column 1 (ret=%d)\n", (int) ret);
        cleanup(henv, hdbc, hstmt, s);
        exit(1);
    }

    ret = SQLFetch(hstmt);
    unsigned int count = 1;
    
    char fsOne[24];
    char fsTwo[128];

    while (ret != SQL_NO_DATA) {
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
            fprintf(stderr, "Unable to fetch row (ret=%d)\n", (int) ret);
            cleanup(henv, hdbc, hstmt, s);
            exit(1);
        }

        sprintf(fsOne, "File%04d", count);
        sprintf(fsTwo, "%s=%s", fsOne, dest);
        fprintf(s, "%s\n", fsTwo);
        count++;
        ret = SQLFetch(hstmt);
    }

    if (count == 1) {
        memcpy(dest, "default_video.mp4", 0x12);
        sprintf(fsOne, "File%04d", count);
        sprintf(fsTwo, "%s=./%s", fsOne, dest);
        fprintf(s, "%s\n", fsTwo);
    }

    cleanup(henv, hdbc, hstmt, s);
    return 0;
}
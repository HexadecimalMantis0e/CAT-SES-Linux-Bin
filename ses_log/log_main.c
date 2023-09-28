#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include "util.h"
#include "shared.h"
#include "log_main.h"

int log_main(int conflict, int argCount, const char *arguments[]) {
    char logMsg[2064];
    logMsg[2047] = '\0';

    char src[24];
    src[7] = '\0';
    strcpy(logMsg, "Unknown");

    char logType[16];
    strcpy(logType, "UNKNOWN");
    strcpy(src, "MAIN");

    char *slashPtr = strrchr(arguments[0], '/');
    unsigned int destLen = (slashPtr - arguments[0]) + 1;
    
    char dest[512];

    if (slashPtr) {
        strncpy(dest, arguments[0], destLen);
        dest[destLen] = '\0';
    }
    else {
        dest[0] = '\0';
    }

    if (argCount == 2) {
        strncpy(logMsg, arguments[1], 2048);
    }

    if (argCount > 2) {
        strncpy(logType, arguments[1], 8);
        strncpy(logMsg, arguments[2], 2048);
    }

    if (argCount > 3) {
        strncpy(src, arguments[3], 8);
    }

    logType[8] = '\0';
    logMsg[2048] = '\0';

    char name[64];

    if (gethostname(name, 64)) {
        puts("gethostname failed.....Why?");
        return 1;
    }

    get_time();
    t_convert = localtime(&rawtime);

    if (!t_convert) {
        printf("\nError converting time");
        return 1;
    }

    char s[208];
    char mdy[208];

    if (!strftime(s, 200, "%Y/%m/%d %H:%M:%S", t_convert) || !strftime(mdy, 200, "%m/%d/%Y", t_convert)){
        printf("\nError converting time(strftime)");
        return 1;
    }

    int day = t_convert->tm_mday;
    t_earlier = rawtime - 86400;
    t_convert = localtime(&t_earlier);

    if (!t_convert) {
        printf("\nError converting earlier search time(localtime)");
        return 1;
    }

    char ymd[208];

    if (!strftime(ymd, 200, "%Y%m%d_", t_convert)) {
        printf("\nError converting earlier search time(strftime)");
        return 1;
    }

    char filename[256];
    strcpy(filename, dest);
    strcat(filename, "../log/");
    strcat(filename, src);
    strcat(filename, "_");

    if (conflict) {
        strcat(filename, "CONFLICT_");
    }

    strcat(filename, name);
    strcat(filename, ".log");

    struct stat status;
    FILE *stream;

    if (stat(filename, &status)) {
        stream = fopen(filename, "w");

        if (!stream) {
            printf("1. Error opening log file %s \n", filename);
            return 1;
        }
    }
    else {
        stat(filename, &status);
        t_convert = localtime(&status.st_mtim.tv_sec);

        if (t_convert->tm_mday == day && abs(status.st_mtim.tv_sec - rawtime) <= 86400) {
            stream = fopen(filename, "a");

            if (!stream) {
                puts("3. Error opening log file");
                return 1;
            }
        }

        else {
            char newa[256];
            strcpy(newa, dest);
            strcat(newa, "../log/");
            strcat(newa, ymd);
            strcat(newa, src);
            strcat(newa, "_");
            strcat(newa, name);
            strcat(newa, ".log");

            if (rename(filename, newa)) {
                printf("Error renaming log file from %s to %s\n", filename, newa);
                return 1;
            }

            stream = fopen(filename, "w");

            if (!stream) {
                puts("2. Error opening log file");
                return 1;
            }
        }

        fprintf(stream, "%s\t\t%s\t\t%s\n", s, logType, logMsg);
        fclose(stream);    
    }
    return 0;
}
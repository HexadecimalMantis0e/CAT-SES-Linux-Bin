#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "shared.h"
#include "util.h"

int short_nap(int amount) {
    struct timespec requested_time;
    requested_time.tv_sec = 0;
    requested_time.tv_nsec = 1000000 * amount;
    return nanosleep(&requested_time, NULL);
}

void get_time() {
    time(&rawtime);
    ptr_tm = gmtime(&rawtime);
    utc_tm = *ptr_tm;
    ptr_tm = localtime(&rawtime);
    local_tm = *ptr_tm;
}
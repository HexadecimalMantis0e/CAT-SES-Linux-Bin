#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern struct tm *t_convert;
extern time_t t_fileday;
extern struct tm *ptr_tm;
extern time_t rawtime;
extern time_t t_today;
extern struct tm local_tm;
extern struct tm utc_tm;
extern struct tm t_struct;
extern time_t t_earlier;
extern time_t t_value;

#endif
#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern struct tm t_struct;
extern struct tm utc_tm;
extern struct tm local_tm;
extern time_t t_value;
extern struct tm *ptr_tm;
extern struct tm *t_convert;
extern time_t t_earlier;
extern time_t rawtime;
extern time_t t_today;

#endif
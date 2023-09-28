#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "shared.h"

struct tm *t_convert;
time_t t_fileday;
struct tm *ptr_tm;
time_t rawtime;
time_t t_today;
struct tm local_tm;
struct tm utc_tm;
struct tm t_struct;
time_t t_earlier;
time_t t_value;

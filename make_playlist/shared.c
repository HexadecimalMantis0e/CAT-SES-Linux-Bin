#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "shared.h"

struct tm t_struct;
struct tm utc_tm;
struct tm local_tm;
time_t t_value;
struct tm *ptr_tm;
struct tm *t_convert;
time_t t_earlier;
time_t rawtime;
time_t t_today;
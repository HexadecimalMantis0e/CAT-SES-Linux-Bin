#include <stdio.h>
#include <stdlib.h>
#include "log_main.h"
#include "util.h"

int main(int argc, const char *argv[], const char *envp[]) {
    int count = 0;

    while (1) {
        int flag = log_main(0, argc, argv);
        count++;

        if (flag) {
            short_nap(100);
        }
        
        if (!flag || count > 9) {
            if (flag) {
                if (log_main(1, argc, argv)) {
                    exit(1);
                }
            }
            exit(0);
        }
    }
}
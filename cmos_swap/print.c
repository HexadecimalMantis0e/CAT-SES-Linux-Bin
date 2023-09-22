#include <stdio.h>
#include <stdlib.h>
#include "print.h"

void print_head_warn() {
    puts("\n\n\nCMOS Read/Write for AMOS 3001");
    puts("=============================\n");
    puts("WARNING: This application can write directly into the CMOS memory which");
    puts("         contains BIOS settings for this unit. It was written specifically for");
    puts("         the AMOS 3001 and may not work correctly on other systems. The input file");
    puts("         should be created by this application and NOT BE TAMPERED WITH!!!!");
    puts("         Writing an incorrect or altered file may result in placing the unit in");
    puts("         an non-bootable state (bricked)\n");
}

void print_info() {
    puts("         To use this application, enter directive with optional filename.");
    puts("         Default filename = cmos.mem\n         File name must be 8 characters or less with optional 3 character extension");
    puts("         Directive:");
    puts("               -r for reading from CMOS into a disk file");
    puts("               -w for writing to CMOS from a disk file\n");
    puts("         Example:\n              cmos_swap -r my_cmos.mem\n                       would store the CMOS contents in file my_cmos.mem");
    puts("              cmos_swap -w\n                       would copy the contents of file cmos.mem(default filename) to system CMOS memory\n\n");
}
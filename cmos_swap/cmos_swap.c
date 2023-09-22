#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/io.h>
#include "util.h"
#include "print.h"

int main(int argc, const char *argv[], const char *envp[]) {
    char src[24];
    strcpy(src, "cmos.mem");
    int mode = 0;
    print_head_warn();

    if (argc <= 1 || argc > 3) {
        puts("==================================================");
        puts("Error: Missing or too many command line arguments.<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
        puts("==================================================");
        print_info();
        exit(8);
    }

    if (strcmp(argv[1], "-r") && strcmp(argv[1], "-w")) {
        puts("==================================================");
        puts("Error: Unknown directive.<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
        puts("==================================================");
        print_info();
        exit(8);
    }

    if (!strcmp(argv[1], "-w")) {
        mode = 1;
    }

    char dest[208];

    if (argc <= 2) {
        strncpy(dest, src, 12);
    }

    else {
        if (strlen(argv[2]) > 127) {
            puts("==================================================");
            puts("Error: Filename is too long.<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
            puts("==================================================");
            print_info();
            exit(8);
        }
        strcpy(dest, argv[2]);
    }

    uint8_t *ptr = (uint8_t *) malloc(0x100);

    char modes[15];

    if (mode) {
        memcpy(modes, "r", 2);
    }
    else {
        memcpy(modes, "w", 2);
    }

    FILE *stream = fopen(dest, modes);

    if (!stream) {
        puts("==================================================");
        printf("Cannot open %s<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n", dest);
        puts("==================================================");
        exit(8);
    }
    iopl(3);
    cli();

    if (mode) {
        printf("Writing file %s contents to CMOS\n", dest);
        stream = fopen(dest, "r");

        for (int i = 0; i < 256; i++) {
            ptr[i] = fgetc(stream);
        }

        uint8_t csOne = fgetc(stream);
        uint8_t csTwo = checksum(ptr, 256);

        if (csTwo != csOne) {
            puts("==================================================");
            printf("Check sum error: file %s has been corrupted <<<<<<<<<<\n", dest);
            puts("==================================================");
            exit(8);
        }
        printf("\n checksum: %d\n\n", csOne);

        for (int i = 0; i < 256; i++) {
            int flag = 0; 

            for (int j = 0; j < 7; j++) {
                uint8_t nums[16] = { 0, 2, 4, 6, 7, 8, 9 };

                if (nums[j] == i ) {
                    flag = 1;
                }
            }

            if (flag) {
                printf("XXX.");
            }
            else {
                printf("%d.", ptr[i]);
                setCmos(i, ptr[i]);
            }
        }
        puts("\n");
    }
    else {
        printf("Reading CMOS and saving to file %s\n", dest);

        for (int i = 0; i < 256; i++) {
            ptr[i] = readCmos(i);
            printf("%d.", ptr[i]);
        }

        puts("\n");
        uint8_t csOne = checksum(ptr, 256);
        printf("\n checksum: %d\n\n", csOne);

        for (int i = 0; i < 256; i++) {
            fputc(ptr[i], stream);
        }
        fputc(csOne, stream);
    }

    sti();
    iopl(0);
    fclose(stream);
    free(ptr);
    exit(0);
}

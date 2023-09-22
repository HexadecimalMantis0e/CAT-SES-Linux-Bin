#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/io.h>
#include "util.h"

void cli() {
    __asm__ ("cli"::);
}

void sti() {
    __asm__ ("sti"::);
}

uint8_t checksum(uint8_t *fb, int count) {
    uint8_t cs = 0;
    
    for (int i = 0; i < count; i++) {
        cs ^= *(fb + i);
    }
    return cs;
}

void setCmos(uint8_t pos, uint8_t val) {
    outb_p(pos, 0x74);
    outb_p(val, 0x75);
}

uint8_t readCmos(uint8_t pos) {
    outb_p(pos, 0x74);
    return (uint8_t) inb_p(0x75);
}
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void cli();
void sti();
uint8_t checksum(uint8_t *fb, int count);
void setCmos(uint8_t pos, uint8_t val);
uint8_t readCmos(uint8_t pos);

#endif

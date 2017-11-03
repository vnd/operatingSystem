#ifndef VGA_H
#define VGA_H

#include <stdarg.h>
#include <stdint.h>
#include "port.h"
#include <stdbool.h>
#include <stdarg.h>

void monitor_init();
void monitor_write(char* str);
void monitor_clear();

#endif	/* VGA_H */
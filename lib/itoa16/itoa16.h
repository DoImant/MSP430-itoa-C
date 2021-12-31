#ifndef _ITOA16_H_
#define _ITOA16_H_

#include <stdint.h>
#include "strrev.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define MAX_DIVIDX 4
#define BASE 10U
#define BUFFER_SIZE  7U
//#define DIVISOR 10000       // Größtmöglicher Divisor bei 16Bit

// Deklaration eines Funktionszeigers   
typedef  void (*fP)(const char c);

extern char* itoa16(char* const buffer, int16_t val);
extern char* uitoa16(char* const buffer, uint16_t val);
extern void itoa16nb( fP putChar, int16_t val);
extern void uitoa16nb( fP putChar, uint16_t val);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif
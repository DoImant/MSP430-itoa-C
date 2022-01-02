#ifndef _ITOA32_H_
#define _ITOA32_H_

#include <stdint.h>
#include "strrev.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define BASE_L 10UL

// declaration of a function pointer.
typedef  void (*fP)(const char c);

extern char* itoa32(char* const buffer, int32_t val);
extern char* uitoa32(char* const buffer, uint32_t val);
extern void itoa32nb( fP putChar, int32_t val);
extern void uitoa32nb( fP putChar, uint32_t val);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif
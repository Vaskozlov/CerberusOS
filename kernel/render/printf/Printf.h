#ifndef Printf_h
#define Printf_h

#include <stdarg.h>
#include "PrintTypes.h"

__BEGIN_DECLS

void *memset (void *__s, int __c, size_t __n) __THROW __nonnull ((1));

/**
 * Printf - prints anything
 * @__fmt: the format string
 *
 * Return: number of bytes printed
 */
int Printf(unsigned int printWay, const char *__restrict __fmt, ...);

__END_DECLS

#endif /* Printf_h */

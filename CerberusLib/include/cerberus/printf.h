#ifndef cerbPrintf_h
#define cerbPrintf_h

#include <stdarg.h>
#include <cerberus/cerberus.h>
#include <cerberus/printTypes.h>

__BEGIN_DECLS

int cerbPrintFloat(double value, PrintingParams_t *params);
int cerbPrintINT(uintmax_t value, PrintingParams_t *params, const char printWay);

int cerbPuts(const char *__restrict __str);
int cerbPrintString(const char *__restrict __str);
int cerbPrintf(const char *__restrict __fmt, ...);

__END_DECLS

#endif /* cerbPrintf_h */

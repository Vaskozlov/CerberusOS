#include <float.h>
#include <cerberus/cerberus.h>

__BEGIN_DECLS

typedef union {
    u64 value;
    double floatingPoint;
} IEEE754u64_t; 

typedef union {
    i32 value;
    float floatingPoint;
} IEEE754u32_t; 

float cerbABSf(float x);
double cerbABS(double x);

__END_DECLS

#include <cerberus/cerbMath.h>

float cerbABSf(float x){
    IEEE754u32_t ieee754;
    ieee754.floatingPoint = x;
    ieee754.value &= (~((u32)0) ^ ((u32)1 << 31));
    return ieee754.floatingPoint;
}

double cerbABS(double x){
    IEEE754u64_t ieee754;
    ieee754.floatingPoint = x;
    ieee754.value &= (~((u64)0) ^ ((u64)1 << 63));
    return ieee754.floatingPoint;
}

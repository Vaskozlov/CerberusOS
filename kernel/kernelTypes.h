#ifndef kernelTypes_h
#define kernelTypes_h

#include <stddef.h>
#include <inttypes.h>

#if defined(MAJOR_VERSION)
#  define CERBERUS_MAJOR_VERSION MAJOR_VERSION
#else
#  define CERBERUS_MAJOR_VERSION 0
#endif

#if defined(MINOR_VERSION)
#  define CERBERUS_MINOR_VERSION MINOR_VERSION
#else
#  define CERBERUS_MINOR_VERSION 0
#endif

#if defined(PATCH_VERSION)
#  define CERBERUS_PATCH_VERSION MINOR_VERSION
#else
#  define CERBERUS_PATCH_VERSION 0
#endif

#define _NEW 1

#include <cerberus/cerberus.h>

#endif /* kernelTypes_h */

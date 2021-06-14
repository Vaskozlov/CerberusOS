#ifndef cerberus_h
#define cerberus_h

#if defined(__clang__)
#  define COMPILER_NAME "clang"
#elif defined(__GNUC__)
#  define COMPILER_NAME "gcc"
#endif

#if defined(__cplusplus)
#  include <cerberus/cerberusc++.hpp>
#else
#  include <cerberus/cerberusc.h>
#endif /* __cplusplus */

#endif /* cerberus_h */

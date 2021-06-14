#ifndef cerbPrintTypes_h
#define cerbPrintTypes_h

#include <cerberus/cerberus.h>

__BEGIN_DECLS

#define POWERS_POSITVE_OFFSET 300
#define OUTPUT_ARRAY_SIZE (sizeof(uintmax_t) * 9)

enum PrintingWays{
    BinNum      = 'b',
    OctNum      = 'o',
    HexNum      = 'x',
    HexNumBig   = 'X',
    PointNum    = 'p'
};

typedef struct PrintingParams{
    u8 negative     : 1;
    u8 minus_flag   : 1;
    u8 plus_flag    : 1;
    u8 hashtag_flag : 1;
    u8 zero_flag    : 1;
    u8 whitespace   : 1;

    unsigned width;
    unsigned precision;
} PrintingParams_t;

#define INIT_PRINTING_PARAMS {0, 0, 0, 0, 0, 0, 0, ~0U}

extern int (*CPutchar)(int);
extern void (*CSetColor)(u8 r, u8 g, u8 b);
extern char ConverterBuffer[OUTPUT_ARRAY_SIZE];
extern const double PowersOf10[601];

const char *bufConvertINT(intmax_t value, const char printWay, char buffer[OUTPUT_ARRAY_SIZE]);
const char *bufConvertUINT(uintmax_t value, const char printWay, char buffer[OUTPUT_ARRAY_SIZE]);
const char *bufConvertFloat(double value, char buffer[OUTPUT_ARRAY_SIZE]);
const char *bufConvertFloatMan(double value, char buffer[OUTPUT_ARRAY_SIZE]);

#define ConvertINT(value, printWay)     bufConvertINT(value, printWay, ConverterBuffer)
#define ConvertUINT(value, printWay)    bufConvertUINT(value, printWay, ConverterBuffer)
#define ConvertFloat(value)             bufConvertFloat(value, ConverterBuffer)
#define ConvertFloatMan(value)          bufConvertFloatMan(value, ConverterBuffer)

__END_DECLS

#endif /* cerbPrintTypes_h */

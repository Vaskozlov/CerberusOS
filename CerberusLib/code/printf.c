#include <alloca.h>
#include <cerberus/cerbMath.h>
#include <cerberus/printf.h>

#define isnum(x) (x >= '0' && x <= '9')
#define isinf(x) __builtin_isinf (x)
#define isnan(x) __builtin_isnan (x)

extern double log10(double x);
extern double round(double x);

enum PrintingSizes{
    SIZE_DEFAULT    = 0,
    SIZE_h          = 1,
    SIZE_hh         = 2,
    SIZE_l          = 3,
    SIZE_ll         = 4,
    SIZE_j          = 5,
    SIZE_z          = 6,
    SIZE_t          = 7,
    SIZE_L          = 8
};

int cerbPuts(const char *__restrict __str){
    int result = cerbPrintString(__str);
    CPutchar('\n');
    return result + 1;
}

int cerbPrintString(const char *__restrict __str){
    const char *__save = __str;
    for (; *__str != '\0'; __str++) CPutchar(*__str);
    return (int)(__save - __str);
}

static unsigned getFlags(const char *__restrict __fmt, PrintingParams_t *params){

    switch (*__fmt){
        case '+':
            params->plus_flag = 1;      return 1U;

        case '-':
            params->minus_flag = 1;     return 1U;

        case '#':
            params->hashtag_flag = 1;   return 1U;

        case ' ':
            params->whitespace = 1;     return 1U;

        case '0':
            params->zero_flag = 1;      return 1U;

        default:
            return 0U;
    }
}

static int getWidth(const char *__restrict __fmt, PrintingParams_t *params){
    if (*__fmt == '*') return -1;

    int offset = 0;

    for (; isnum(*__fmt); __fmt++, offset++){
        params->width = (int)((unsigned)params->width * 10U);
        params->width += *__fmt - '0';
    }

    return offset;
}

static int getPrecision(const char *__restrict __fmt, PrintingParams_t *params){
    if (*__fmt != '.') return 0;
    __fmt++;
    if (*__fmt == '*') return -1;

    int offset = 2;

    if (!isnum(*__fmt)) return 1;
    params->precision = *__fmt - '0'; // is not zero, so we have to set it
    
    for (__fmt++; isnum(*__fmt); __fmt++, offset++){
        params->precision = (int)((unsigned)params->precision * 10U);
        params->precision += *__fmt - '0';
    }

    return offset;
}

static unsigned int getSize(const char *__restrict __fmt){

    switch (*__fmt){
        case 'h': return SIZE_h + (*(__fmt + 1) == 'h');
        case 'l': return SIZE_l + (*(__fmt + 1) == 'l');
        case 'j': return SIZE_j;
        case 'z': return SIZE_z;
        case 't': return SIZE_t;
        case 'L': return SIZE_L;
        default:  return SIZE_DEFAULT;
    }
}

int cerbPrintINT(uintmax_t value, PrintingParams_t *params, const char printWay){
    char *buffer        = (char*) alloca(OUTPUT_ARRAY_SIZE);
    char *__str         = (char*) bufConvertUINT(value, printWay, buffer);
    unsigned int change = (unsigned int) ((buffer + OUTPUT_ARRAY_SIZE - 1) - __str);
    unsigned int extras = 0, lazy_ptr = 0;
    int numLength       = change;
    char lazy_str[4]    = {0};
    const char cher2put = params->zero_flag ? '0' : ' ';

    lazy_str[lazy_ptr++] =  '-' * (params->negative) +
                            '+' * (!params->negative && params->plus_flag) +
                            ' ' * (params->whitespace && !params->negative);
    extras += lazy_str[0] != '0';

    if (params->hashtag_flag || printWay == 'p'){
        switch (printWay){
            case 'b':
                lazy_str[lazy_ptr++] = '0'; lazy_str[lazy_ptr++] = 'b'; extras += 2; break;
            
            case 'o':
                lazy_str[lazy_ptr++] = '0'; lazy_str[lazy_ptr++] = 'o'; extras += 2; break;
            
            case 'p':
            case 'x':
                lazy_str[lazy_ptr++] = '0'; lazy_str[lazy_ptr++] = 'x'; extras += 2; break;
            
            case 'X':
                lazy_str[lazy_ptr++] = '0'; lazy_str[lazy_ptr++] = 'X'; extras += 2; break;
            
            default: break;
        }
    }

    int __tmp = (int) MIN((int)params->width - (int)params->precision - extras, (int)params->width - (int)change - extras);
    change += extras;
    change += __tmp + MAX(0, ((int)params->precision - (int)numLength));
    
    if (params->minus_flag == 0)
        for (int i = 0; i < __tmp; i++) CPutchar(cher2put);
    
    for (int i = 0; lazy_str[i] != '\0'; i++) CPutchar(lazy_str[i]);
    for (int i = 0; i < ((int)params->precision - (int)numLength); i++) CPutchar('0');
    for (; *__str != '\0'; __str++) CPutchar(*__str);

    if (params->minus_flag)
        for (int i = 0; i < __tmp; i++) CPutchar(cher2put);

    return change;
}

int cerbPrintFloat(double value, PrintingParams_t *params){
    if (isinf(value)){
        int printed = 3;
        for (; printed < (int) params->width; printed++) CPutchar(' ');
        CPutchar('i'); CPutchar('n'); CPutchar('f');
        return printed;
    }
    else if (isnan(value)){
        int printed = 3;
        for (; printed < (int) params->width; printed++) CPutchar(' ');
        CPutchar('n'); CPutchar('a'); CPutchar('n');
        return printed;
    }

    u64 printingLocal;
    double copy4value;
    int digits, printingPlan, printingResult, printedDigits = 0;
    char *buffer = (char*) alloca(OUTPUT_ARRAY_SIZE);
    const char lazy_char = ('-' * (value < 0.0)) + ('+' * (value >= 0.0 && params->plus_flag)) + (' ' * (value >= 0.0 && params->whitespace));
    
    value = cerbABS(value);
    digits = (int) log10(value);
    printingPlan = MAX(0, digits) + params->precision + (params->precision != 0) + (lazy_char != '\0') + 1;
    printingResult = MAX((int)params->width, printingPlan);

    if (!params->minus_flag)
        for (int j = printingPlan; j < (int)params->width; j++)
            CPutchar(' ');

    if (lazy_char != '\0') CPutchar(lazy_char);

    if (value < 1.0) CPutchar('0');
    while (value >= 1.0){
        if (value < (double)(UINT64_MAX)){
            char *__ptr;
            
            printingLocal = (u64) value;
            __ptr = (char*) bufConvertUINT(printingLocal, 'u', buffer);
            printedDigits += (buffer + OUTPUT_ARRAY_SIZE - 1) - __ptr;

            for (; *__ptr != '\0'; __ptr++) CPutchar(*__ptr);

            value -= (double) printingLocal;
        }
        else{
            char *__ptr;
            int extra_zeros;
            double divider = PowersOf10[POWERS_POSITVE_OFFSET + digits - 18];
            
            copy4value = value;
            value /= divider;

            printingLocal = (u64) value;
            __ptr = (char*) bufConvertUINT(printingLocal, 'u', buffer);
            extra_zeros = (buffer + OUTPUT_ARRAY_SIZE - 1) - __ptr;

            for (; *__ptr != '\0'; __ptr++) CPutchar(*__ptr);
            for (extra_zeros = 20 - extra_zeros; extra_zeros > 0; extra_zeros--) CPutchar('0');

            copy4value -= (double) printingLocal * divider;
            value = copy4value / 1e18;
            printedDigits += 20;
        }
    }

    for (; printedDigits <= digits; printedDigits++) CPutchar('0');

    if (params->precision != 0){
        CPutchar('.');
        char *__ptr;
        int afterPoint = params->precision, extra_zeros;

        while (afterPoint > 0){
            if (afterPoint <= 18){
                int actualAfterPoint;

                printingLocal = round(value * PowersOf10[POWERS_POSITVE_OFFSET + afterPoint]);
                __ptr = (char*) bufConvertUINT(printingLocal, 'u', buffer);
                actualAfterPoint = (buffer + OUTPUT_ARRAY_SIZE - 1) - __ptr;

                for (; actualAfterPoint < afterPoint; actualAfterPoint++) CPutchar('0');
                for (; *__ptr != '\0'; __ptr++) CPutchar(*__ptr);
                afterPoint = 0;
            }
            else{
                copy4value = value * 1e19;
                printingLocal = (u64)copy4value;
                __ptr = (char*) bufConvertUINT(printingLocal, 'u', buffer);
                extra_zeros = (buffer + OUTPUT_ARRAY_SIZE - 1) - __ptr;

                for (extra_zeros = 19 - extra_zeros; extra_zeros > 0; extra_zeros--) CPutchar('0');
                for (; *__ptr != '\0'; __ptr++) CPutchar(*__ptr);
                afterPoint -= 19;
            }
        }
    }

    if (params->minus_flag)
        for (int j = printingPlan; j < (int)params->width; j++)
            CPutchar(' ');

    return printingResult;
}

static int cerbPrintFloatSci(double value, PrintingParams_t *params){
    char *__ptr;
    int change;
    char *buffer = (char*) alloca(OUTPUT_ARRAY_SIZE);
    const char lazy_char = ('-' * (value < 0.0)) + ('+' * (value >= 0.0 && params->plus_flag)) + (' ' * (value >= 0.0 && params->whitespace));
    
    value = cerbABS(value);
    __ptr = (char*) bufConvertFloatMan(value, buffer);
    const char *iter = __ptr;

    if (!params->minus_flag && params->width != 0){
        
        for (; *iter != '\0'; iter++);
        change = iter - __ptr;

        for (int i = 0; i < MAX(0, (int)params->width - change - (lazy_char != '\0')); i++)
            CPutchar(' ');
    }

    if (lazy_char != '\0') CPutchar(lazy_char);
    for (; *__ptr != '\0'; __ptr++) CPutchar(*__ptr);
    change = __ptr - iter;

    if (params->minus_flag){
        for (int i = 0; i < MAX(0, (int)params->width - change - (lazy_char != '\0')); i++)
            CPutchar(' ');
    }

    return MAX(change, (int) params->width);
}

int cerbPrintf(const char *__restrict __fmt, ...){
    va_list args;
    va_start(args, __fmt);
    int printed_chars = 0;

    if (__fmt == 0x0 || CPutchar == 0x0) return -1;

    for (; *__fmt != '\0'; __fmt++){
        switch (*__fmt){
            case '%':

                if (*(__fmt + 1) == '%'){
                    __fmt++;
                    printed_chars++;
                    CPutchar('%');
                    break;
                }

                int widthOf, precisionOf;
                PrintingParams_t params = INIT_PRINTING_PARAMS;
                __fmt++;

                __fmt += getFlags(__fmt, &params);
                
                widthOf = getWidth(__fmt, &params);
                if (widthOf == -1) { params.width = va_arg(args, int); __fmt++; }
                else __fmt += widthOf;

                precisionOf = getPrecision(__fmt, &params);
                if (precisionOf == -1) { params.precision = va_arg(args, int); __fmt += 2; }
                else __fmt += precisionOf;

                unsigned size;
                {
                    static const unsigned BytesOfSize[] = {
                        0, // SIZE_DEFAULT
                        1, 2, // SIZE_h and SIZE_hh
                        1, 2, // SIZE_l and SIZE_ll
                        1, 1, // SIZE_j and SIZE_z
                        1, 1 // SIZE_t and SIZE_L
                    };
                    size = getSize(__fmt);
                    __fmt += BytesOfSize[size];
                }

                switch (*__fmt){
                    case 'd':
                    case 'i':
                        {
                            intmax_t value;
                            params.precision = params.precision == ~0U ? 0 : params.precision;

                            switch (size){
                                case SIZE_l:    value = va_arg(args, long); break;
                                case SIZE_ll:   value = va_arg(args, long long); break;
                                case SIZE_j:    value = va_arg(args, intmax_t); break;
                                case SIZE_z:    value = va_arg(args, size_t); break;
                                case SIZE_t:    value = va_arg(args,ptrdiff_t); break;
                                default:        value = va_arg(args, int); break;
                            }

                            params.negative = value < 0;
                            value = ABS(value);
                            printed_chars += cerbPrintINT(value, &params, *__fmt);
                        }
                        break;

                    case 'p':
                        size = SIZE_j;
                    case 'b':
                    case 'o':
                    case 'x':
                    case 'u':
                    case 'X':
                        {
                            uintmax_t value;
                            params.precision = params.precision == ~0U ? 0 : params.precision;

                            switch (size){
                                case SIZE_l:    value = va_arg(args, unsigned long); break;
                                case SIZE_ll:   value = va_arg(args, unsigned long long); break;
                                case SIZE_j:    value = va_arg(args, uintmax_t); break;
                                case SIZE_z:    value = va_arg(args, size_t); break;
                                case SIZE_t:    value = va_arg(args,ptrdiff_t); break;
                                default:        value = va_arg(args, unsigned); break;
                            }
                            printed_chars += cerbPrintINT(value, &params, *__fmt);
                        }
                        break;

                    case 'n':
                        switch (size){
                            case SIZE_hh:   *va_arg(args, char*)        = printed_chars;
                            case SIZE_h:    *va_arg(args, short*)       = printed_chars;
                            case SIZE_l:    *va_arg(args, long*)        = printed_chars;
                            case SIZE_ll:   *va_arg(args, long long*)   = printed_chars;
                            case SIZE_j:    *va_arg(args, intmax_t*)    = printed_chars;
                            case SIZE_z:    *va_arg(args, size_t*)      = printed_chars;
                            case SIZE_t:    *va_arg(args, ptrdiff_t*)   = printed_chars;
                            default:        *va_arg(args, int*)         = printed_chars;
                        }
                        break;

                    case 'c':
                        CPutchar(va_arg(args, int));
                        printed_chars++;
                        break;

                    case 's':
                        {
                            const char *__ptr = va_arg(args, const char*);
                            if (params.precision == ~0U) for (; *__ptr != '\0'; __ptr++) CPutchar(*__ptr);
                            else for (const char *i = __ptr; i < __ptr + params.precision && *__ptr != '\0'; __ptr++) CPutchar(*__ptr);
                        }

                        break;

                    case 'F':
                    case 'f':
                        params.precision = params.precision == ~0U ? 6 : params.precision;
                        printed_chars += cerbPrintFloat(va_arg(args, double), &params);
                        break;

                    case 'e':
                    case 'E':
                        params.precision = params.precision == ~0U ? 6 : params.precision;
                        printed_chars += cerbPrintFloatSci(va_arg(args, double), &params);
                        break;
                }
                break;

            case '\xFF':
                if (CSetColor != NULL){
                    CSetColor(*((unsigned char*)__fmt + 1), *((unsigned char*)__fmt + 2), *((unsigned char*)__fmt + 3));
                    __fmt += 3;
                }
                break;

            default:
                CPutchar(*__fmt);
                printed_chars++;
                break;
        }
    }

    return printed_chars;
}


#include "PrintTypes.h"

/**
 *  Function to print char
 */
int (*PutcharWay[8])(int) = {0x0};
void (*SetColorWay[8])(unsigned char r, unsigned char g, unsigned char b) = {0x0};
char NumberBuffer[127];

/**
 *  Hex values in lower bound and upperbound forms
 */
const char *hex_values[] = {"0123456789abcdef", "0123456789ABCDEF"};

/**
 *  Powers of 10. From 10^0 to 10^300
 */
const double powerof10[] = {
    1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7,
    1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14,
    1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20,
    1e21, 1e22, 1e23, 1e24, 1e25, 1e26, 1e27,
    1e28, 1e29, 1e30, 1e31, 1e32, 1e33, 1e34,
    1e35, 1e36, 1e37, 1e38, 1e39, 1e40, 1e41,
    1e42, 1e43, 1e44, 1e45, 1e46, 1e47, 1e48,
    1e49, 1e50, 1e51, 1e52, 1e53, 1e54, 1e55,
    1e56, 1e57, 1e58, 1e59, 1e60, 1e61, 1e62,
    1e63, 1e64, 1e65, 1e66, 1e67, 1e68, 1e69, 
    1e70, 1e71, 1e72, 1e73, 1e74, 1e75, 1e76, 
    1e77, 1e78, 1e79, 1e80, 1e81, 1e82, 1e83, 
    1e84, 1e85, 1e86, 1e87, 1e88, 1e89, 1e90, 
    1e91, 1e92, 1e93, 1e94, 1e95, 1e96, 1e97, 
    1e98, 1e99, 1e100, 1e101, 1e102, 1e103, 1e104, 
    1e105, 1e106, 1e107, 1e108, 1e109, 1e110, 1e111, 
    1e112, 1e113, 1e114, 1e115, 1e116, 1e117, 1e118, 
    1e119, 1e120, 1e121, 1e122, 1e123, 1e124, 1e125, 
    1e126, 1e127, 1e128, 1e129, 1e130, 1e131, 1e132, 
    1e133, 1e134, 1e135, 1e136, 1e137, 1e138, 1e139, 
    1e140, 1e141, 1e142, 1e143, 1e144, 1e145, 1e146, 
    1e147, 1e148, 1e149, 1e150, 1e151, 1e152, 1e153, 
    1e154, 1e155, 1e156, 1e157, 1e158, 1e159, 1e160, 
    1e161, 1e162, 1e163, 1e164, 1e165, 1e166, 1e167, 
    1e168, 1e169, 1e170, 1e171, 1e172, 1e173, 1e174, 
    1e175, 1e176, 1e177, 1e178, 1e179, 1e180, 1e181, 
    1e182, 1e183, 1e184, 1e185, 1e186, 1e187, 1e188, 
    1e189, 1e190, 1e191, 1e192, 1e193, 1e194, 1e195, 
    1e196, 1e197, 1e198, 1e199, 1e200, 1e201, 1e202, 
    1e203, 1e204, 1e205, 1e206, 1e207, 1e208, 1e209, 
    1e210, 1e211, 1e212, 1e213, 1e214, 1e215, 1e216, 
    1e217, 1e218, 1e219, 1e220, 1e221, 1e222, 1e223, 
    1e224, 1e225, 1e226, 1e227, 1e228, 1e229, 1e230, 
    1e231, 1e232, 1e233, 1e234, 1e235, 1e236, 1e237, 
    1e238, 1e239, 1e240, 1e241, 1e242, 1e243, 1e244, 
    1e245, 1e246, 1e247, 1e248, 1e249, 1e250, 1e251, 
    1e252, 1e253, 1e254, 1e255, 1e256, 1e257, 1e258, 
    1e259, 1e260, 1e261, 1e262, 1e263, 1e264, 1e265, 
    1e266, 1e267, 1e268, 1e269, 1e270, 1e271, 1e272, 
    1e273, 1e274, 1e275, 1e276, 1e277, 1e278, 1e279, 
    1e280, 1e281, 1e282, 1e283, 1e284, 1e285, 1e286, 
    1e287, 1e288, 1e289, 1e290, 1e291, 1e292, 1e293, 
    1e294, 1e295, 1e296, 1e297, 1e298, 1e299, 1e300
};

/**
 *   FillDecBuffer - fills NumberBuffer with string version of value in dec form
 *  @value: number to print
 *  @buffer_pointer: position in NumberBuffer (default is sizeof(NumberBuffer) - 1)
 *  @printed_bytes: pointer to printed_bytes counter
 *  Return: updated buffer_pointer
 */
static int FillDecBuffer(unsigned long long value, unsigned buffer_pointer, int *printed_bytes)
{
    do{
        NumberBuffer[buffer_pointer--] = value % 10 + '0';
        value /= 10;
        (*printed_bytes)++;
    } while(value != 0);

    return buffer_pointer;
}

/**
 *   FillHexBuffer - fills NumberBuffer with string version of value in hex form
 *  @value: number to print
 *  @buffer_pointer: position in NumberBuffer (default is sizeof(NumberBuffer) - 1)
 *  @printed_bytes: pointer to printed_bytes counter
 *  @use_uppercase: if it is set, func will print upper bounds chars 
 *  Return: updated buffer_pointer
 */
static int FillHexBuffer(unsigned long long value, unsigned buffer_pointer, int *printed_bytes, int use_uppercase)
{
    do{
        NumberBuffer[buffer_pointer--] = hex_values[use_uppercase][(value & 0b1111)];
        value >>= 4;
        (*printed_bytes)++;
    } while(value != 0);

    return buffer_pointer;
}

/**
 *   FillOctBuffer - fills NumberBuffer with string version of value in oct form
 *  @value: number to print
 *  @buffer_pointer: position in NumberBuffer (default is sizeof(NumberBuffer) - 1)
 *  @printed_bytes: pointer to printed_bytes counter
 *  Return: updated buffer_pointer
 */
static int FillOctBuffer(unsigned long long value, unsigned buffer_pointer, int *printed_bytes)
{
    do{
        NumberBuffer[buffer_pointer--] = (value & 0b111) + '0';
        value >>= 3;
        (*printed_bytes)++;
    } while(value != 0);

    return buffer_pointer;
}

/**
 *  See Printf.h
 */
void mem8set(void *dest, i8 value, size_t times){
	char *pointer = dest;

	for (; pointer != (char*)dest + times; pointer++)
		*pointer = value;
}

/**
 *  See Printf.h
 */
int PrintUInt(parameters_t *param, unsigned long long value, char print_type, unsigned int printWay){
    int (*Putchar)(int) = PutcharWay[printWay];
    
    if (print_type == 'p' && value == 0x0)
    {
        Putchar('('); Putchar('n'); Putchar('i'); Putchar('l'); Putchar(')');
        return 4;
    }

    char lazy_char = '\0';
    int number_size, printed_bytes = 0;
    unsigned i = sizeof(NumberBuffer) - 1;

    if (param->precision == -1)
        param->precision++; // for different print functions precision means different things, but for PrintUint default is 0

    switch (print_type)
    {
        case 'X':
            i = FillHexBuffer(value, i, &printed_bytes, 1);

            if (param->hashtag_flag){
                NumberBuffer[i--] = 'X';
                NumberBuffer[i--] = '0';
                printed_bytes += 2;
            }
            
            break;

        case 'p':

        case 'x':
            i = FillHexBuffer(value, i, &printed_bytes, 0);
            
            if (param->hashtag_flag){
                NumberBuffer[i--] = 'x';
                NumberBuffer[i--] = '0';
                printed_bytes += 2;
            }

            break;

        case 'o':
            i = FillOctBuffer(value, i, &printed_bytes);

            if (param->hashtag_flag){
                NumberBuffer[i--] = '0';
                printed_bytes += 1;
            }

            break;

        default:
            i = FillDecBuffer(value, i, &printed_bytes);
            break;
    }

    number_size = printed_bytes;

    if (param->negative)
        lazy_char = '-';

    else if (param->plus_flag)
        lazy_char = '+';

    if (param->minus_flag != 1){
        for (int j = param->width, end = printed_bytes + param->precision + (lazy_char != '\0') - 1; j > end; j--){
            printed_bytes++;
            Putchar(' ');
        }
    }

    if (lazy_char != '\0'){
        Putchar(lazy_char);
        printed_bytes++;
    }

    for (int j = number_size; j < param->precision; j++){
        Putchar('0');
        printed_bytes++;
    }

    while (i < (sizeof(NumberBuffer) - 1)){
        Putchar(NumberBuffer[++i]);
    }

    if (param->minus_flag == 1){
        for (; printed_bytes < param->width; printed_bytes++)
            Putchar(' ');
    }

    return printed_bytes;
}

/**
 *  See PrintF.h
 */
int PrintFloat(parameters_t *param, double value, unsigned int printWay)
{
    int (*Putchar)(int) = PutcharWay[printWay];
    char lazy_char = '\0';

    if (isinf(value)){
        Putchar('i'); Putchar('n'); Putchar('f');
        return 3;
    } else if (isnan(value)){
        Putchar('n'); Putchar('a'); Putchar('n');
        return 3;
    }

    if (value < 0.0){
        value = -value;
        lazy_char = '-';
    } else if (param->plus_flag){
        lazy_char = '+';
    }

    if (param->precision == -1)
        param->precision = 6;

    unsigned i;
    int printed_bytes = 0, printed_digits = 0;
    int digits = MAX((int)log10(value), 1); // calculate log10 for value to find number of digits in it
    int planning2print = digits + param->precision + (param->precision != 0) + (lazy_char != '\0');

    uint64_t value2prrint; // maximum 1e20
    double value_copy;

    if (param->minus_flag == 0){
        for (int j = param->width; j > planning2print; j--, printed_bytes++)
            Putchar(' ');
    }

    if (lazy_char != '\0'){
        printed_bytes++;
    }

    while (value >= 1.0){
        if (value <= 1.8e19){
            value2prrint = (uint64_t)value;
            i = FillDecBuffer(value2prrint, sizeof(NumberBuffer) - 1, &printed_digits);

            while(i < (sizeof(NumberBuffer) - 1))
                Putchar(NumberBuffer[++i]);
            
            value -= (double)value2prrint;
        } 
        else{
            int extra_zeros;
            double divider = powerof10[digits - 18];
            value_copy = value;
            value /= divider;
            
            value2prrint = (uint64_t)value;
            i = FillDecBuffer(value2prrint, sizeof(NumberBuffer) - 1, &printed_digits);
            extra_zeros = (sizeof(NumberBuffer) - 1) - i;

            for (extra_zeros = 20 - extra_zeros; extra_zeros > 0; extra_zeros--, printed_digits++)
                Putchar('0');

            while(i < (sizeof(NumberBuffer) - 1))
                Putchar(NumberBuffer[++i]);

            value_copy -= (double) value2prrint * divider;
            value = value_copy / 1e18;
        }
    }

    printed_bytes += printed_digits;

    for (int j = printed_digits; j < digits; j++){
        Putchar('0');
        printed_bytes++;
    }
    
    if (param->precision != 0){
        Putchar('.');
        printed_bytes++;
        int after_point = param->precision, extra_zeros;
        printed_digits = 0;

        while (after_point > 0){

            if (after_point <= 18){
                value2prrint = (double) value * powerof10[after_point];

                i = FillDecBuffer(value2prrint, sizeof(NumberBuffer) - 1, &printed_bytes);
                extra_zeros = sizeof(NumberBuffer) - i;

                for (int j = after_point - extra_zeros; j >= 0; j--, printed_bytes++){
                    Putchar('0');
                }

                while(i < (sizeof(NumberBuffer) - 1))
                    Putchar(NumberBuffer[++i]);

                for (; printed_bytes < planning2print; printed_bytes++)
                    Putchar('0');

                after_point = 0;
            }
            else{

                value_copy = value * 1e19;
                value2prrint = (uint64_t)value_copy;

                i = FillDecBuffer(value2prrint, sizeof(NumberBuffer) - 1, &printed_bytes);
                extra_zeros = sizeof(NumberBuffer) - 1 - i;

                for (extra_zeros = 19 - extra_zeros; extra_zeros > 0; extra_zeros--, printed_bytes++)
                    Putchar('0');

                while(i < (sizeof(NumberBuffer) - 1))
                    Putchar(NumberBuffer[++i]);       

                value_copy -= (double) value2prrint;
                value = value_copy;
                after_point -= 19;  
            }
        }
    }

    if (param->minus_flag){
        for (; printed_bytes < param->width; printed_bytes++)
            Putchar(' ');
    }

    return printed_bytes;
}

/**
 *  See Printf.h
 */
int PrintString(parameters_t *param, const char *str, unsigned int printWay)
{
    int printed_bytes = 0;
    int (*Putchar)(int) = PutcharWay[printWay];

    if (param->precision == -1)
        param->precision = 1 << 25;
    
    for (; *str != '\0' && param->precision > printed_bytes; str++)
        Putchar(*str);

    return printed_bytes;
}

/**
 *  See Printf.h
 */
void init_parameters(parameters_t *param)
{
	param->negative = 0;
	param->zero_flag = 0;
	param->plus_flag = 0;
	param->minus_flag = 0;
	param->hashtag_flag = 0;
	param->whitespace_flag = 0;
	param->width = 0;
	param->precision = -1;
}

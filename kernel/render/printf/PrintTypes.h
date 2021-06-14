#ifndef PrintTypes_h
#define PrintTypes_h

#include <math.h>
#include <stddef.h>
#include <inttypes.h>
#include <kernelTypes.h>

__BEGIN_DECLS

#define TO_RAD(x) x / 180 * PI
#define TO_DEG(x) x * 180 * PI

#ifndef _MAX
#  define MAX(a, b) ((a) > (b) ? (a) : (b))
#  define MIN(a, b) ((a) < (b) ? (a) : (b))
#  define _MAX 1
#endif

#define SIZE_STANDART 0
#define SIZE_h 1
#define SIZE_hh 2
#define SIZE_l 3
#define SIZE_ll 4
#define SIZE_j 5
#define SIZE_z 6
#define SIZE_t 7
#define SIZE_L 8

typedef int8_t i8;
typedef uint8_t u8;

/**
 * Putchar - print char (You have to set before using Printf!)
 * Return printed char
 */
extern int (*PutcharWay[8])(int);
extern void (*SetColorWay[8])(unsigned char r, unsigned char g, unsigned char b);

/**
 *  parameters - structure with parameters
 * 	@plus_flag: on if plus_flag is specified
 * 	@minus_flag: on if minus_flag is specified
 *  @whitespace_flag: on if whitespace_flag is specified
 *  @hashtag_flag: on if hashtag_flag is specified
 *  @zero_flag: on if zero_flag is specified
 * 	
 *  @width: width specified
 *  @specified: specified specified :D
 */
typedef struct parameters
{
    unsigned short negative         : 1;
	unsigned short minus_flag 		: 1;
	unsigned short plus_flag  		: 1;
	unsigned short whitespace_flag 	: 1;
	unsigned short hashtag_flag 	: 1;
	unsigned short zero_flag 		: 1;
	unsigned short size 			: 8;
	
	int width;
	int precision;
} parameters_t;

/**
 *  Powers of 10. From 10^0 to 10^300
 */
extern const double powerof10[302];

/**
 *  init_parameters - sets zero to all field in parameters_t, except precision which is -1
 *  @param: parameters_t where to set zeroes
 */
void init_parameters(parameters_t *param);

/**
 *  PrintUint - print unisgned values in almost any form
 * 	@param: parameters to print value
 *  @value: value to print on screen
 *  @print_type: similar to C printf specifiers for unsigned/signed values
 *  Return: number of pinted chars
 */
int PrintUInt(parameters_t *param,  unsigned long long value, char print_type, unsigned int printWay);

/**
 *  PrintFloat - prints double (don't use numbers bigger than uintmax_t)
 * 	@param: parameters to print value
 *  @value: value to print on screen
 *  Return: number of pinted chars
 */
int PrintFloat(parameters_t *param, double value, unsigned int printWay);

/**
 *  PrintString - prints string
 * 	@param: parameters to print stirngs
 *  @value: string to print on screen
 *  Return: number of pinted chars
 */
int PrintString(parameters_t *param, const char *str, unsigned int printWay);

__END_DECLS

#endif /* PrintTypes_h */

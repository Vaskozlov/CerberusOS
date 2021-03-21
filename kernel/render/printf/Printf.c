#include "Printf.h"

/**
 *  get_flag - searc fol flags in __fmt
 *  @__fmt: thr format string without first %	
 * 
 *  Return: 1 if any flag was found and 0 when no flags were found
 */ 
int get_flag(const char *__restrict __fmt, parameters_t *param)
{
	
	switch (*__fmt)
	{
		case '+':
			param->plus_flag = 1;
			return 1;

		case '-':
			param->minus_flag = 1;
			return 1;

		case '#':
			param->hashtag_flag = 1;
			return 1;

		case ' ':
			param->whitespace_flag = 1;
			return 1;

		case '0':
			param->zero_flag = 1;
			return 1;

		default:
			return 0;
	}
}

/**
 *  get_width - gets width specified
 * 	@__fmt: format string
 * 	@param: pointer where width will be set
 * 	Return: -1 when width is *, otherwise returns width size
 */ 
int get_width(const char *__restrict __fmt, parameters_t *param)
{
	if (*__fmt == '*')
		return -1;

	int power = 1;
	int offset = 0;

	for (; *__fmt <='9' && *__fmt >= '0'; __fmt++, offset++)
	{
		param->width *= power;
		param->width += *__fmt - '0';
		power *= 10;
	}
	
	return offset;
}

/**
 *  get_precision gets precision specified
 * 	@__fmt: format string
 * 	@param: pointer where width will be set
 *  Return: -1 when width is *, otherwise returns precision size
 */
int get_precision(const char *__restrict __fmt, parameters_t *param)
{
	if (*__fmt != '.')
		return 0;
	
	__fmt++;

	if (*__fmt == '*')
		return -1;

	int offset = 1;

	if (*__fmt <='9' && *__fmt >= '0')
		param->precision = 0;

	param->precision += *__fmt - '0';
	__fmt++;
	offset++;

	for (; *__fmt <='9' && *__fmt >= '0'; __fmt++, offset++)
	{
		param->precision *= 10;
		param->precision += *__fmt - '0';
	}

	return offset;
}

int get_size(const char *__restrict __fmt, parameters_t *param)
{

	switch (*__fmt)
	{
		case 'h':

			if (*(__fmt + 1) == 'h'){
				param->size = SIZE_hh;
				return 2;
			}

			param->size = SIZE_h;
			return 1;

		case 'l':
			if (*(__fmt + 1) == 'l'){
				param->size = SIZE_ll;
				return 2;
			}

			param->size = SIZE_l;
			return 1;

		case 'j':
			param->size = SIZE_j;
			return 1;

		case 'z':
			param->size = SIZE_z;
			return 1;

		case 't':
			param->size = SIZE_t;
			return 1;

		case 'L':
			param->size = SIZE_L;
			return 1;

		default:
			param->size = SIZE_STANDART;
			return 0;
	}
}

int Printf(const char *__restrict __fmt, ...)
{
	if (__fmt == 0x0 || Putchar == 0x0)
		return -1;

	va_list args;
	va_start(args, __fmt);
	int printed_bytes = 0;

	for (; *__fmt != '\0'; __fmt++){

		if (*__fmt == '%'){
			__fmt++;

			if (*__fmt == '%'){
				Putchar('%');
				printed_bytes++;
				continue;
			}

			int local; // fol local small saves
			parameters_t param;
			init_parameters(&param);

			__fmt += get_flag(__fmt, &param);
			local = get_width(__fmt, &param);

			if (local == -1){
				param.width = va_arg(args, int);
				__fmt++;
			}
			else{
				__fmt += local;
			}

			local = get_precision(__fmt, &param);
			
			if (local == -1){
				param.precision = va_arg(args, int);
				__fmt += 2;
			}
			else{
				__fmt += local;
			}

			__fmt += get_size(__fmt, &param);

			switch (*__fmt){

				case 'd':

				case 'i':
					{
						long long int value;

						switch (param.size)
						{
							case SIZE_l:
								value = (long) va_arg(args, long);
								break;

							case SIZE_ll:
								value = va_arg(args, long long);
								break;
							
							case SIZE_j:
								value = va_arg(args, intmax_t);
								break;
								
							case SIZE_z:
								value = va_arg(args, size_t);
								break;
							
							case SIZE_t:
								value = va_arg(args, ptrdiff_t);
								break;

							default:
								value = va_arg(args, int);
						}

						if (value < 0){
							param.negative = 1;
							value = -value;
						}

						printed_bytes += PrintUInt(&param, (unsigned long long) value, *__fmt);
					}
					break;

				case 'u':

				case 'o':

				case 'X':
				
				case 'x':
					{
						unsigned long long value;

						switch (param.size)
						{
							case SIZE_l:
								value = (long) va_arg(args, unsigned long);
								break;

							case SIZE_ll:
								value = va_arg(args, unsigned long long);
								break;

							default:
								value = va_arg(args, unsigned int);
								break;
						}

						printed_bytes += PrintUInt(&param, value, *__fmt);
					}
					break;

				case 'f':
					
				case 'F':
					printed_bytes += PrintFloat(&param, va_arg(args, double));
					break; 
					
				case 'e':

				case 'E':

				case 'g':

				case 'G':

				case 'a':

				case 'A':
					break;

				case 's':
					PrintString(&param, va_arg(args, const char *));
					break;

				case 'c':
					Putchar(*__fmt);
					printed_bytes++;
					break;

				case 'p':
					mem8set(&param, 0, sizeof(param));
					param.hashtag_flag = 1;
					printed_bytes += PrintUInt(&param, (uintmax_t)va_arg(args, void*), *__fmt);
					break;

				case 'n':
					switch (param.size)
					{
						case SIZE_hh:
							*va_arg(args, char*) = printed_bytes;
							break;

						case SIZE_h:
							*va_arg(args, short*) = printed_bytes;
							break;

						case SIZE_l:
							*va_arg(args, long*) = printed_bytes;
							break;

						case SIZE_ll:
							*va_arg(args, long long*) = printed_bytes;
							break;

						case SIZE_j:
							*va_arg(args, intmax_t*) = printed_bytes;
							break;

						case SIZE_z:
							*va_arg(args, size_t*) = printed_bytes;
							break;
						
						case SIZE_t:
							*va_arg(args, ptrdiff_t*) = printed_bytes;
							break;

						default:
							*va_arg(args, int*) = printed_bytes;
					}
					break;
			}
		}
		else{
			Putchar(*__fmt);
			printed_bytes++;
		}
	}

	va_end(args);

	return printed_bytes;
}

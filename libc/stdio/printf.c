#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if(*format == 'd') {
			format++;
			int num = va_arg(parameters, int);
			if(num == 0) {
				char buffer[1];
				buffer[0] =(char)'0';
				if(!print((const char*)buffer, 1))
					return -1;

				if(maxrem < 1)
					return -1;

				written += 1;
				bzero(&buffer, 1);
			} else {
				char buffer[256];
				bool negative = num < 0 ? true : false;
				if (negative) num *= -1;
				size_t i = 0;
				while(num) {
					int rem = num %10;
					num -= rem;
					num /= 10;
					buffer[i] = (char)(rem + '0');
					i++;
				}
				
				if(negative) {
					buffer[i] = (char)'-';
					i++;
				}
				
				strrev(buffer);

				if(maxrem < i)
					return -1;

				if(!print((const char*)buffer, i))
					return -1;
				written += i;
				bzero(&buffer, 256);
			}
		} else if(*format == 'u') {
			format++;
			uint32_t num = va_arg(parameters, uint32_t);
			if(num == 0) {
				char buffer[1];
				buffer[0] =(char)'0';
				if(!print((const char*)buffer, 1))
					return -1;

				if(maxrem < 1)
					return -1;

				written += 1;
				bzero(&buffer, 1);
			} else {
				char buffer[256];
				size_t i = 0;
				while(num) {
					int rem = num %10;
					num -= rem;
					num /= 10;
					buffer[i] = (char)(rem + '0');
					i++;
				}
				
				strrev(buffer);

				if(maxrem < i)
					return -1;

				if(!print((const char*)buffer, i))
					return -1;
				written += i;
				bzero(&buffer, 256);
			}
		} else if(*format == 'x') {
			format++;
			uint32_t num = va_arg(parameters, uint32_t);

			if(num == 0) {
				char buffer[1];
				buffer[0] =(char)'0';
				if(!print((const char*)buffer, 1))
					return -1;
				
				if(maxrem < 1)
					return -1;

				written += 1;
				bzero(&buffer, 1);
			} else {
				char buffer[256];
				size_t i = 0;
				while(num) {
					int rem = num % 16;
					if (rem < 10)
						buffer[i] = (char)('0' + rem);
					else
						buffer[i] = (char)('7' + rem);
					num /= 16;
					i++;
				}
				
				if(maxrem < i)
					return -1;

				strrev(buffer);
				if(!print((const char*)buffer, i))
					return -1;
				written += 1;
				bzero(&buffer, 256);
			}
		} else if(*format == 'C') {
			format++;
			int num = va_arg(parameters, int);
			color(num);
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}

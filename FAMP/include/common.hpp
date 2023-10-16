#ifndef FAMP_PROTOCOL_COMMON_H
#define FAMP_PROTOCOL_COMMON_H

#ifndef OS_RELATED
#include <iostream>
#include <cstring>
#include <unistd.h> /* TODO: Do we need this? */
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef char                 int8;
typedef unsigned char        uint8;
typedef int8*                pint8;
typedef uint8*               puint8;
typedef const int8           cint8;
typedef const uint8          cuint8;
typedef cint8*               cpint8;
typedef cuint8*              cpuint8;

typedef short                int16;
typedef unsigned short       uint16;
typedef int16*               pint16;
typedef uint16*              puint16;
typedef const int16          cint16;
typedef const uint16         cuint16;
typedef cint16*              cpint16;
typedef cuint16*             cpuint16;

typedef int                  int32;
typedef unsigned int         uint32;
typedef int32*               pint32;
typedef uint32*              puint32;
typedef const int32          cint32;
typedef const uint32         cuint32;
typedef cint32*              cpint32;
typedef cuint32*             cpuint32;

typedef long long            int64;
typedef unsigned long long   uint64;
typedef int64*               pint64;
typedef uint64*              puint64;
typedef const int64          cint64;
typedef const uint64         cuint64;
typedef cint64*              cpint64;
typedef cuint64*             cpuint64;

typedef int64                size;
typedef uint64               usize;
typedef const size           csize;
typedef const usize          cusize;

#ifdef __cplusplus
}
#endif

#ifndef __cplusplus
typedef uint8               bool;

#ifndef true
#define true                0x01
#endif

#ifndef false
#define false               0x00
#endif
#endif

/* `OS_RELATED` will be defined in C/C++ programs that are a part of the OS. */
#ifndef OS_RELATED
#define FAMP_ASSERT(cond, msg, ...)         \
if(!(cond)) {                               \
    fprintf(stderr, msg, ##__VA_ARGS__);    \
    exit(EXIT_FAILURE);                     \
}

#define FAMP_LOG(msg, ...)                  \
fprintf(stderr, msg, ##__VA_ARGS__);

#define FAMP_ERROR(msg, ...)                \
FAMP_LOG(msg, ##__VA_ARGS__)                \
exit(EXIT_FAILURE);

pint8 initiate_path(int8 data1[], int8 data2[])
{
	/* `data1`, at least, has to be valid and not NULL. */
	FAMP_ASSERT(data1, "Cannot initiate the path. No data given to configure the path.\n")

	/* Initiate `array` and concat `data1` to it. */
	int8 *array = (int8 *) calloc(strlen(data1), sizeof(*array));
	strcat(array, data1);

	/* If `data2` is not NULL, go ahead and perform the same action as above. */
	if(data2 != NULL)
	{
		array = (int8 *) realloc(array, ((strlen(data1) + 1) + strlen(data2)) * sizeof(*array));
		
		FAMP_ASSERT(strlen(data1) + strlen(data2) <= 80, 
			"Path is too large: %s.\nFAMP only allows up to 50 characters for a path.\n", strcat(array, data2))
		
		strcat(array, data2);
	}

	return array;
}
#endif

/* `int8_make_abs` - get rid of the six `F`s at the beginning of the signed value.
 *
 * Example: lets say we have `char a = 0xAB`. Its value will be represented as `FFFFFFAB`.
 * 			`int8_make_abs` will take `FFFFFFAB` and get `AB` and disregard the `F`s.
 * */
#define int8_make_abs(int8_val)			(uint8)(int8_val & 0xFF)

/* `int16_make_abs` - get rid of the four `F`s at the beginning of the signed value.
 *
 * Example: lets say we have `short a = 0xABCD`. Its value will be represented as `FFFFABCD`.
 *			`int16_make_abs` will take `FFFFABCD` and get `ABCD` and disregard the `F`s.
 * */
#define int16_make_abs(int16_val)		(uint16)(int16_val & 0xFFFF)

/* `make_absolute` implies the same exact functionality as the above macros, except it supports integers. */
template<typename T, typename R>
#ifndef OS_RELATED
	requires (std::is_same<T, int8>::value
		|| std::is_same<T, int16>::value
		|| std::is_same<T, int32>::value /* Not needed, but I went ahead and added this because why not. */)
		&& (std::is_same<R, uint8>::value
		||	std::is_same<R, uint16>::value
		||	std::is_same<R, uint32>::value /* Again, not needed but I went ahead and added this because why not. */)
#endif
#ifndef OS_RELATED
R make_absolute(T &value)
#else
R make_absolute(T value)
#endif
{
	/* Some type checking first just to be safe. */
	if(sizeof(T) != sizeof(R))
		return (T) value;
	
	switch(sizeof(T))
	{
		case 1: return (R) value & 0xFF;break;
		case 2: return (R) value & 0xFFFF;break;
		case 4: return (R) value & 0xFFFFFFFF;break;
		default: break;
	}

	return (R) value;
}

template<typename T>
#ifndef OS_RELATED
    requires (std::is_same<T, uint16>::value
		|| std::is_same<T, int16>::value
        || std::is_same<T, uint32>::value
		|| std::is_same<T, int32>::value)
        && (!std::is_same<T, uint8>::value)
#endif
#ifndef OS_RELATED
T revert_value(T &value)
#else
T revert_value(T value)
#endif
{
    #ifdef OS_RELATED
    if(sizeof(T) == 1) return (T) value & 0xFF;
    #endif

    T old_value = value;
    value ^= value;

    switch(sizeof(T))
    {
        case 2: {
            value |= (value << 0) | ((old_value >> 0) & 0xFF);
            value = (value << 8) | ((old_value >> 8) & 0xFF);
            break;
        }
        case 4: {
            value |= (value << 0) | ((old_value >> 0) & 0xFF);
            value = (value << 8) | ((old_value >> 8) & 0xFF);
            value = (value << 8) | ((old_value >> 16) & 0xFF);
            value = (value << 8) | ((old_value >> 24) & 0xFF);
            break;
        }
        default: break;
    }

    return (T) value;
}

#ifdef OS_RELATED
#define __START __attribute__((section("__start")))

#ifndef BIT32_PROGRAM
asm(".code16gcc");

void read_in_memory(uint16 addr, uint8 start_sector, uint8 sector_amount)
{
    uint16 a = (0x02 << 8) | sector_amount;

    /* Read from disk. This will be replaced when ATA PIO support is added. */
    __asm__("mov ax, %0" : : "dN"((uint16)addr));
    __asm__("mov es, ax\nxor bx, bx\nmov ax, %0" : : "d"((uint16) a));
    __asm__("mov ch, 0x00\nmov cl, %0" : : "dN"((uint8) start_sector));
    __asm__("mov dh, 0x00\nmov dl, 0x80\nint 0x13");

    return;
}
#endif

#ifdef BIT32_PROGRAM
/*
 *  __inp: back-end function
 *
 *  Read-in a value from a port
 *
 *  Input: uint16 port
 *  Output: uint8 rv(return-value from the port)
 *  On Error: This function does not error
 */
uint8 __inp(uint16 port) {
    uint8 rv;
    
    __asm__ __volatile__("in %0, %1": "=a"(rv): "dN"(port));
    return rv;
}
/*
 *  __outp: back-end function
 *
 *  Write a value to a port
 *
 *  Input: uint16 port, uint8 data
 *  Output: None
 *  On Error: This function does not error
 */
void __outp(uint16 port, uint8 data) {
    __asm__ __volatile__("outb %0, %1":: "dN"(port), "a"(data));
}
#endif

#endif

#endif
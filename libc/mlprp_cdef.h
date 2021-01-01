#ifndef __mlprp_cdef__
#define __mlprp_cdef__

typedef unsigned long size_t;
typedef signed char char_t;
typedef unsigned char uchar_t;
typedef signed int wchar_t;
typedef unsigned int uwchar_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed long int32_t;
typedef signed long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool;

static inline void outb(uint16_t port, uint8_t val) {
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef null
#define null 0
#endif

#ifndef nullptr
#define nullptr 0x00000000
#endif

#ifndef endl
#define endl "\n"
#endif

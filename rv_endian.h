#ifndef _RV_ENDIAN_H_
#define _RV_ENDIAN_H_

//  如果不是大端则移位成大端

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define GREV_BE32(x) (x)
#else
	//  grev(x, 0x18) or rev8
#define GREV_BE32(x) (	\
	(((x) & 0xFF000000) >> 24) | (((x) & 0x00FF0000) >> 8)  | \
	(((x) & 0x0000FF00) << 8)  | (((x) & 0x000000FF) << 24))
#endif

//  向左循环移位
static inline uint32_t rol32(uint32_t x, uint32_t n)
{
	return ((x) << n) | ((x) >> (32 - n));
}

//  little-endian loads and stores (unaligned)

static inline uint32_t get32u_le(const uint8_t* v)
{
	return ((uint32_t)v[0]) | (((uint32_t)v[1]) << 8) |
		(((uint32_t)v[2]) << 16) | (((uint32_t)v[3]) << 24);
}

static inline void put32u_le(uint8_t* v, uint32_t x)
{
	v[0] = x;
	v[1] = x >> 8;
	v[2] = x >> 16;
	v[3] = x >> 24;
}

//  big-endian loads and stores (unaligned)
// 大端数据调用与存储

static inline uint32_t get32u_be(const uint8_t* v)
{
	return (((uint32_t)v[0]) << 24) | (((uint32_t)v[1]) << 16) |
		(((uint32_t)v[2]) << 8) | ((uint32_t)v[3]);
}

static inline void put32u_be(uint8_t* v, uint32_t x)
{	
	/*
	* 使得存储变为大端输入"ABC" = 0b00636261
	* :param x:
	* :param V
	*/
	v[0] = x >> 24;
	v[1] = x >> 16;
	v[2] = x >> 8;
	v[3] = x;
}

static inline uint64_t get64u_be(const uint8_t* v)
{
	return (((uint64_t)v[0]) << 56) | (((uint64_t)v[1]) << 48) |
		(((uint64_t)v[2]) << 40) | (((uint64_t)v[3]) << 32) |
		(((uint64_t)v[4]) << 24) | (((uint64_t)v[5]) << 16) |
		(((uint64_t)v[6]) << 8) | ((uint64_t)v[7]);
}

static inline void put64u_be(uint8_t* v, uint64_t x)
{
	v[0] = x >> 56;
	v[1] = x >> 48;
	v[2] = x >> 40;
	v[3] = x >> 32;
	v[4] = x >> 24;
	v[5] = x >> 16;
	v[6] = x >> 8;
	v[7] = x;
}

#endif

#include "bitmanip.h"

//  carryless multiply

uint32_t rv32b_clmul(uint32_t rs1, uint32_t rs2)		// PASS
{
	/*
	* 输入两个数据，依据小端格式进行两数相乘
	* :param rs1		:	第一个乘数（小端），0d17 = 0x00000011
	* :param rs2		:	第二个乘数（小端）, 0d5	 = 0x00000005
	* :return x			:	两数的乘积（小端）, 0d85 = 0x00000055
	*/
	uint32_t x = 0;
	for (int i = 0; i < 32; i++)
		if ((rs2 >> i) & 1)
			x ^= rs1 << i;
	return x;
}

uint32_t rv32b_clmulh(uint32_t rs1, uint32_t rs2)				// UT
{
	uint32_t x = 0;
	for (int i = 1; i < 32; i++)
		if ((rs2 >> i) & 1)
			x ^= rs1 >> (32 - i);
	return x;
}

uint32_t rv32b_clmulr(uint32_t rs1, uint32_t rs2)				// PASS
{
	/*
	* 输入两个数据，依据小端格式进行两数相乘（左右取反）
	* :param rs1		:	第一个乘数（小端取反），0d17 = 0x88000000
	* :param rs2		:	第二个乘数（小端取反）, 0d5	 = 0xA0000000
	* :return x			:	两数的乘积（小端取反）, 0d85 = 0xAA000000
	*/
	uint32_t x = 0;
	for (int i = 0; i < 32; i++)
		if ((rs2 >> i) & 1)
			x ^= rs1 >> (32 - i - 1);
	return x;
}

//  rotate right ROR / RORI

uint32_t rv32b_ror(uint32_t rs1, uint32_t rs2)				// PASS
{
	/*
	* 输入两个数据，求rs1循环右移rs2个单位以后的结果
	* :param rs1		:	被位移的数据		rs1 = 0x000ABCD0		rs1 = 0x000ABCD0
	* :param rs2		:	移动的位数			rs2 = 0x00000003		rs2 = 0x0000000A
	* :return 			:	循环移动后的结果	res = 0x0001579A		res = 0x340002AF
	*
	* 注：正常算法中第一个数据求非，本例中第二个数据求非
	*/
	int shamt = rs2 & (32 - 1);
	return (rs1 >> shamt) | (rs1 << ((32 - shamt) & (32 - 1)));
}

uint32_t rv32b_rol(uint32_t rs1, uint32_t rs2)
{
	int shamt = rs2 & 31;
	return (rs1 << shamt) | (rs1 >> ((32 - shamt) & 31));
}


//  and with negate ANDN

uint64_t rv32b_andn(uint32_t rs1, uint32_t rs2)					// PASS
{
	/*
	* 输入两个数据，求两个数的按位与非值(32 bits)
	* :param rs1		:	第一个数据（不求非的那个）	rs1 = 0xCCCCCCCC
	* :param rs2		:	第二个数据（求非的那个）	rs2 = 0xAAAAAAAA, ~rs2 = 0x55555555
	* :return 			:	两个数据的与非值			res = 0x44444444
	*
	* 注：正常算法中第一个数据求非，本例中第二个数据求非
	*/
	return rs1 & ~rs2;
}

uint32_t rv32b_orn(uint32_t rs1, uint32_t rs2)
{
	return rs1 | ~rs2;
}


uint32_t rv32b_xnor(uint32_t rs1, uint32_t rs2)
{
	return rs1 ^ ~rs2;
}

//gorc and gorci
uint32_t rv32b_gorc(uint32_t rs1, uint32_t rs2)
{
	uint32_t x = rs1;
	int shamt = rs2 & 31;
	if(shamt & 1) x |= ((x & 0x55555555) << 1) | ((x & 0xAAAAAAAA) >> 1);
	if(shamt & 2) x |= ((x & 0x33333333) << 2) | ((x & 0xCCCCCCCC) >> 2);
	if(shamt & 4) x |= ((x & 0x0F0F0F0F) << 4) | ((x & 0xF0F0F0F0) >> 4);
	if(shamt & 8) x |= ((x & 0x00FF00FF) << 8) | ((x & 0xFF00FF00) >> 8);
	if(shamt &16) x |= ((x & 0x0000FFFF) <<16) | ((x & 0xFFFF0000) >>16);
	return x;
}



//  generalized reverse GREV / GREVI
uint32_t rv32b_grev(uint32_t rs1, uint32_t rs2)					// PASS
{
	/*
	* 输入一个数据，按照rs2给定的规则取反
	* :param rs1		:	被移位的数据												0x12ABCDEF
	* :param rs2		:	值为 0 - 31 中的一个值，超出范围不影响结果，取其前32个值	0b18
	* :return x			:	移位以后的结果												0xFEDCBA21
	*
	* 注：SHA2-256 中常用rs2 = 0x18 = 0b10010，即为按将数据按十六进制取逆
	*/
	uint32_t x = rs1;
	int shamt = rs2 & 31;
	if (shamt & 1)
		x = ((x & 0x55555555) << 1) | ((x & 0xAAAAAAAA) >> 1);
	if (shamt & 2)
		x = ((x & 0x33333333) << 2) | ((x & 0xCCCCCCCC) >> 2);
	if (shamt & 4)
		x = ((x & 0x0F0F0F0F) << 4) | ((x & 0xF0F0F0F0) >> 4);
	if (shamt & 8)
		x = ((x & 0x00FF00FF) << 8) | ((x & 0xFF00FF00) >> 8);
	if (shamt & 16)
		x = ((x & 0x0000FFFF) << 16) | ((x & 0xFFFF0000) >> 16);
	return x;
}

//  32-bit helper for SHFL/UNSHFL

static inline uint32_t shuffle32_stage(uint32_t src, uint32_t ml,
	uint32_t mr, int n)
{
	uint32_t x = src & ~(ml | mr);
	x |= ((src << n) & ml) | ((src >> n) & mr);
	return x;
}

//  generalized shuffle SHFL / SHFLI

uint32_t rv32b_shfl(uint32_t rs1, uint32_t rs2)
{
	uint32_t x = rs1;
	int shamt = rs2 & 15;

	if (shamt & 8)
		x = shuffle32_stage(x, 0x00FF0000, 0x0000FF00, 8);
	if (shamt & 4)
		x = shuffle32_stage(x, 0x0F000F00, 0x00F000F0, 4);
	if (shamt & 2)
		x = shuffle32_stage(x, 0x30303030, 0x0C0C0C0C, 2);
	if (shamt & 1)
		x = shuffle32_stage(x, 0x44444444, 0x22222222, 1);

	return x;
}

//  generalized unshuffle UNSHFL / UNSHFLI

uint32_t rv32b_unshfl(uint32_t rs1, uint32_t rs2)
{
	uint32_t x = rs1;
	int shamt = rs2 & 15;

	if (shamt & 1)
		x = shuffle32_stage(x, 0x44444444, 0x22222222, 1);
	if (shamt & 2)
		x = shuffle32_stage(x, 0x30303030, 0x0C0C0C0C, 2);
	if (shamt & 4)
		x = shuffle32_stage(x, 0x0F000F00, 0x00F000F0, 4);
	if (shamt & 8)
		x = shuffle32_stage(x, 0x00FF0000, 0x0000FF00, 8);

	return x;
}

uint32_t rv32b_pack(uint32_t rs1, uint32_t rs2)
{
	uint32_t lower = (rs1 << 16) >> 16;
	uint32_t upper = (rs2 << 16);

	return lower | upper;
}


uint32_t rv32b_packu(uint32_t rs1, uint32_t rs2)
{
	uint32_t lower = rs1 >> 16;
	uint32_t upper = (rs2 >> 16) << 16;

	return lower | upper;
}

uint32_t rv32b_packh(uint32_t rs1, uint32_t rs2)
{
	uint32_t lower = rs1 & 255;
	uint32_t upper = (rs2 & 255) << 8;

	return lower | upper;
}



uint32_t xperm(uint32_t rs1, uint32_t rs2, int sz_log2)
{
	uint32_t r = 0;
	uint32_t sz = 1LL << sz_log2;
	uint32_t mask = (1LL << sz) - 1;
	for (int i = 0; i < 32; i+= sz)
	{
	
		uint32_t pos = ((rs2 >> i) & mask) << sz_log2;
		if(pos < 32)
			r |= ((rs1 >> pos) & mask) << i;
	}
	
	return r;
}

uint32_t rv32b_xperm_n(uint32_t rs1, uint32_t rs2)
{
	return xperm(rs1,rs2,2);
}

uint32_t rv32b_xperm_b(uint32_t rs1, uint32_t rs2)
{
	return xperm(rs1,rs2,3);
}

uint32_t rv32b_xperm_h(uint32_t rs1, uint32_t rs2)
{
	return xperm(rs1,rs2,4);
}

uint32_t rv32b_xperm_w(uint32_t rs1, uint32_t rs2)
{
	return xperm(rs1,rs2,5);
}







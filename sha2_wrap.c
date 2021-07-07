#include <string.h>
#include "sha2_wrap.h"
#include "rv_endian.h"

//  pointers to the compression functions

void (*sha256_compress)(void* s) = &rv32_sha256_compress;
void (*sha512_compress)(void* s) = &rv32_sha512_compress;

//  shared part between SHA-224 and SHA-256

static void sha256pad(uint32_t* s, const uint8_t* k, size_t klen, uint8_t pad, const void* in, size_t inlen) {
	/*
	* :param s			:			输入的初始哈希值
	* :param k			:			输入的秘钥长度
	* :param klen		:			如果是HMAC模式，则klen为对应长度，如果是非HMAC模式，则klen为0
	* :param pad		:			pad的内容，标准hash：0x00
	* :param in			:			输入的字符串值，eg："abc"
	* :param inlen		:			输入字符串的字节数，eg：0x00000003
	*/
	size_t i;											// 初始化迭代变量
	uint64_t x;											// 
	uint8_t* mp = (uint8_t*)&s[8];						// s为初始哈希值空间，当前指向新的空空间，mp = message_pad 补充以后的消息空间
	const uint8_t* ip = in;								// 将输入信息初始化

	x = inlen << 3;							//  length in bits

	if (k != NULL) {						//  该变量是为HMAC预留的函数     //
		x += 512;
		for (i = 0; i < klen; i++)
			mp[i] = k[i] ^ pad;
		memset(mp + klen, pad, 64 - klen);
		sha256_compress(s);
	}

	while (inlen >= 64) {						// 如果剩余长度大于64个字节/512比特，则将当前512比特赋予mp
		memcpy(mp, ip, 64);
		sha256_compress(s);						// 对当前字节进行打包
		inlen -= 64;							// 总长度减少64个字节
		ip += 64;								// 目标指针前移64个字节
	}
	memcpy(mp, ip, inlen);						// 不足64个字节的部分压缩打包
	mp[inlen++] = 0x80;							// 先补一个0x1000_0000
	if (inlen > 56) {
		memset(mp + inlen, 0x00, 64 - inlen);	// 剩余长度比64大，则先少补几组0x00
		sha256_compress(s);
		inlen = 0;								// 剩余长度置0
	}

	i = 64;									//  将要处理的字节长度
	while (x > 0) {							
		mp[--i] = x & 0xFF;
		x >>= 8;
	}
	memset(mp + inlen, 0x00, i - inlen);
	sha256_compress(s);
}

//  SHA-224 initial values H0, Sect 5.3.2.
static const uint32_t sha2_224_h0[8] = {
	0xC1059ED8, 0x367CD507, 0x3070DD17, 0xF70E5939,
	0xFFC00B31, 0x68581511, 0x64F98FA7, 0xBEFA4FA4
};

//  Compute 28-byte message digest to "md" from "in" which has "inlen" bytes

void sha2_224(uint8_t* md, const void* in, size_t inlen)
{
	int i;
	uint32_t s[8 + 24];

	for (i = 0; i < 8; i++)					//  set H0 (IV)
		s[i] = sha2_224_h0[i];

	sha256pad(s, NULL, 0, 0x00, in, inlen);

	for (i = 0; i < 7; i++)					//  store big endian output
		put32u_be(&md[i << 2], s[i]);

}

void hmac_sha2_224(uint8_t* mac, const void* k, size_t klen,
	const void* in, size_t inlen)
{
	int i;						// 迭代变量 doge
	uint32_t s[8 + 16];			// 
	uint8_t t[28], k0[28];		// t暂存内部加密哈希中间值，k0存储加密哈希秘钥

	if (klen > 64) {						//  如果秘钥长度大于512位，则使用sha224将长度转成28个字节
		sha2_224(k0, k, klen);
		k = k0;
		klen = 28;
	}

	for (i = 0; i < 8; i++)					//  从sha2_224_h0中读取第一组常数 H0（IV）- sha2_wrap.c
		s[i] = sha2_224_h0[i];

	sha256pad(s, k, klen, 0x36, in, inlen);	// 使用sha256执行内层加密哈希 - sha2_wrap.c - 输出为s

	for (i = 0; i < 7; i++)					//  将s的值暂存在t变量中
		put32u_be(&t[i << 2], s[i]);
	for (i = 0; i < 8; i++)
		s[i] = sha2_224_h0[i];				//  重新设置H0，从sha2_224_h0中读取第一组常数 H0（IV） - sha2_wrap.c

	sha256pad(s, k, klen, 0x5c, t, 28);		// 使用sha256执行内层加密哈希 - sha2_wrap.c - 使用暂存变量t，输出为s

	for (i = 0; i < 7; i++)					//  store big endian output
		put32u_be(&mac[i << 2], s[i]);
}

//  SHA-256 initial values H0, Sect 5.3.3.

static const uint32_t sha2_256_h0[8] = {
	0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
	0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19
};

//  Compute 32-byte message digest to "md" from "in" which has "inlen" bytes

void sha2_256(uint8_t* md, const void* in, size_t inlen)
{
	/*
	* SHA2-256 顶层调用函数
	* :param md		:	uint8_t md[32] 总长度为256，32个字节。用于哈希算法的输出
	* :param in		:	不限定类型，指代哈希算法的输入
	* :param inlen	:	长度需要与in的字节长度相符，不知道的话可以用readhex - test_hex.c 算出来
	*/
	int i;
	uint32_t s[8 + 16];				// 初始化中间变量 前8个位初始值，后16个为中间值

	for (i = 0; i < 8; i++)						// 为SHA256用的8个H0赋初始值
		s[i] = sha2_256_h0[i];

	sha256pad(s, NULL, 0, 0x00, in, inlen);		// 执行纯sha256哈希算法（无秘钥，pad=0x00）,调用sha256pad - sha2_wrap.c 函数

	for (i = 0; i < 8; i++)						// 将产生的字符转换为大端存储
		put32u_be(&md[i << 2], s[i]);
}

void hmac_sha2_256(uint8_t* mac, const void* k, size_t klen,
	const void* in, size_t inlen)
{
	int i;
	uint32_t s[8 + 16];
	uint8_t t[32], k0[32];

	if (klen > 64) {						//  hash the key if needed
		sha2_256(k0, k, klen);
		k = k0;
		klen = 32;
	}

	for (i = 0; i < 8; i++)					//  set H0 (IV)
		s[i] = sha2_256_h0[i];

	sha256pad(s, k, klen, 0x36, in, inlen);

	for (i = 0; i < 8; i++) {				//  get temporary, reinit
		put32u_be(&t[i << 2], s[i]);
		s[i] = sha2_256_h0[i];				//  set H0 (IV)
	}

	sha256pad(s, k, klen, 0x5c, t, 32);

	for (i = 0; i < 8; i++)					//  store big endian output
		put32u_be(&mac[i << 2], s[i]);
}


//  shared part between SHA-384 and SHA-512

static void sha512pad(uint64_t s[8],
	const uint8_t* k, size_t klen, uint8_t pad,
	const void* in, size_t inlen)
{
	size_t i;
	uint64_t x;

	uint8_t* mp = (uint8_t*)&s[8];
	const uint8_t* ip = in;

	x = inlen << 3;							//  length in bits

	if (k != NULL) {						//  key block for HMAC
		x += 1024;
		for (i = 0; i < klen; i++)
			mp[i] = k[i] ^ pad;
		memset(mp + klen, pad, 128 - klen);
		sha512_compress(s);
	}

	while (inlen >= 128) {					//  full blocks
		memcpy(mp, ip, 128);
		sha512_compress(s);
		inlen -= 128;
		ip += 128;
	}

	memcpy(mp, ip, inlen);					//  last data block
	mp[inlen++] = 0x80;
	if (inlen > 112) {
		memset(mp + inlen, 0x00, 128 - inlen);
		sha512_compress(s);
		inlen = 0;
	}

	i = 128;								//  process length
	while (x > 0) {
		mp[--i] = x & 0xFF;
		x >>= 8;
	}
	memset(mp + inlen, 0x00, i - inlen);
	sha512_compress(s);
}

//  SHA-384 initial values H0, Sect 5.3.4.

static const uint64_t sha2_384_h0[8] = {
	0xCBBB9D5DC1059ED8LL, 0x629A292A367CD507LL,
	0x9159015A3070DD17LL, 0x152FECD8F70E5939LL,
	0x67332667FFC00B31LL, 0x8EB44A8768581511LL,
	0xDB0C2E0D64F98FA7LL, 0x47B5481DBEFA4FA4LL
};

//  Compute 48-byte message digest to "md" from "in" which has "inlen" bytes

void sha2_384(uint8_t* md, const void* in, size_t inlen)
{
	int i;
	uint64_t s[8 + 16];

	for (i = 0; i < 8; i++)					//  set H0 (IV)
		s[i] = sha2_384_h0[i];

	sha512pad(s, NULL, 0, 0x00, in, inlen);

	for (i = 0; i < 6; i++)					//  store big endian output
		put64u_be(&md[i << 3], s[i]);
}

void hmac_sha2_384(uint8_t* mac, const void* k, size_t klen,
	const void* in, size_t inlen)
{
	int i;
	uint64_t s[8 + 16];
	uint8_t t[48], k0[48];

	if (klen > 128) {						//  hash the key if needed
		sha2_384(k0, k, klen);
		k = k0;
		klen = 48;
	}

	for (i = 0; i < 8; i++)					//  set H0 (IV)
		s[i] = sha2_384_h0[i];

	sha512pad(s, k, klen, 0x36, in, inlen);

	for (i = 0; i < 6; i++) {				//  get temporary, reinit
		put64u_be(&t[i << 3], s[i]);
	}
	for (i = 0; i < 8; i++)					//  set H0 (IV)
		s[i] = sha2_384_h0[i];

	sha512pad(s, k, klen, 0x5c, t, 48);

	for (i = 0; i < 6; i++)					//  store big endian output
		put64u_be(&mac[i << 3], s[i]);
}

//  SHA-512 initial values H0, Sect 5.3.5.

static const uint64_t sha2_512_h0[8] = {
	0x6A09E667F3BCC908LL, 0xBB67AE8584CAA73BLL,
	0x3C6EF372FE94F82BLL, 0xA54FF53A5F1D36F1LL,
	0x510E527FADE682D1LL, 0x9B05688C2B3E6C1FLL,
	0x1F83D9ABFB41BD6BLL, 0x5BE0CD19137E2179LL
};

//  Compute 64-byte message digest to "md" from "in" which has "inlen" bytes

void sha2_512(uint8_t* md, const void* in, size_t inlen)
{
	int i;
	uint64_t s[8 + 16];

	for (i = 0; i < 8; i++)					//  set H0 (IV)
		s[i] = sha2_512_h0[i];

	sha512pad(s, NULL, 0, 0x00, in, inlen);

	for (i = 0; i < 8; i++)					//  store big endian output
		put64u_be(&md[i << 3], s[i]);
}

void hmac_sha2_512(uint8_t* mac, const void* k, size_t klen,
	const void* in, size_t inlen)
{
	int i;
	uint64_t s[8 + 16];
	uint8_t t[64], k0[64];

	if (klen > 128) {						//  hash the key if needed
		sha2_512(k0, k, klen);
		k = k0;
		klen = 64;
	}

	for (i = 0; i < 8; i++)					//  set H0 (IV)
		s[i] = sha2_512_h0[i];

	sha512pad(s, k, klen, 0x36, in, inlen);

	for (i = 0; i < 8; i++) {				//  get temporary, reinit
		put64u_be(&t[i << 3], s[i]);
		s[i] = sha2_512_h0[i];				//  set H0 (IV)
	}

	sha512pad(s, k, klen, 0x5c, t, 64);

	for (i = 0; i < 8; i++)					//  store big endian output
		put64u_be(&mac[i << 3], s[i]);
}

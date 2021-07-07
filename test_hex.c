#include "test_hex.h"

//  single hex digit

static int hexdigit(char ch)
{
	/*
	* 输入一个字符，返回字符对应的数值
	* :param ch		:	输入的字符
	* :return		:	字符对应的decimal值
	*/
	if (ch >= '0' && ch <= '9')
		return ch - '0';				// 数字判断格式，返回对应的数值
	if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;			// 大写字母判断格式，返回对应的数值
	if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;			// 小写字母判断格式
	return -1;							// 非法字符报错
}

//  read a hex string of "maxbytes", return byte length

size_t readhex(uint8_t* buf, size_t maxbytes, const char* str)
{
	/*
	* 读取str字符串，规定最大长度为maxbytes，返回字符串长度，将字符串整形至buf空间中
	* :param buf		:		返回的整形字符串
	* :param maxbytes	:		输出字符串的最大长度
	* :param str		:		读取的字符串
	* :return i			:		输入字符串 或者 整形后字符串的长度
	*/
	size_t i;
	int h, l;					// h为低位字节，l为高位字节

	for (i = 0; i < maxbytes; i++) {
		h = hexdigit(str[2 * i]);
		if (h < 0)
			return i;
		l = hexdigit(str[2 * i + 1]);
		if (l < 0)
			return i;
		buf[i] = (h << 4) + l;			
	}

	return i;
}

//  print hexadecimal "data", length "len", with label "lab"
void prthex(const char* lab, const void* data, size_t len)
{
	/*
	* 打印二进制数据data，长度为len，标签为lab
	*/
	size_t i;		// 迭代变量 doge
	uint8_t x;		// 临时字节变量

	printf("[TEST] %s ", lab);					// 测试起始标记
	const char hex[] = "0123456789ABCDEF";		// 二进制数据列表

	for (i = 0; i < len; i++) {
		x = ((const uint8_t*)data)[i];			// data的第i个变量赋值给x
		putchar(hex[(x >> 4) & 0xF]);			// 取x的高4位
		putchar(hex[x & 0xF]);					// 取x的低4位
	}
	putchar('\n');
}

//  check "data" of "len" bytes against a hexadecimal test vector "ref"
int chkhex(const char* lab, const void* data, size_t len, const char* ref)
{
	/*
	* 本函数用于检查产生的哈希值与正确输出值是否相符
	* :param lab		:			label，算法标签，本函数中只用于输出打印
	* :param data		:			data，数据源，本函数中指hash生成的变量，用于和ref比较
	* :param len		:			length，所用hash算法的字节长度/字节个数
	* :param ref		:			reference，用于hash算法参考的（拟定/认定）正确输出值
	*
	* COMMENT			:			本网站用于查询正确输出值（需提前转换成ascii），https://www.qqxiuzi.cn/bianma/sha.php
	*/

	size_t i;				// 迭代通用变量
	uint8_t x;				// 
	int fail = 0;

	// 按字节比较结果
	for (i = 0; i < len; i++) {
		x = ((const uint8_t*)data)[i];
		// 按照字节比较输出结果
		if (hexdigit(ref[2 * i]) != ((x >> 4) & 0xF) || hexdigit(ref[2 * i + 1]) != (x & 0x0F))			// 先比较做四个比特，然后比较右四个比特
		{
			fail = 1;
			break;
		}														// (x >> 4) & 0xF 用于格式化输出 
	}

	if (i == len && hexdigit(ref[2 * len]) >= 0) {
		fail = 1;
	}

	printf("[%s] %s %s\n", fail ? "FAIL" : "PASS", lab, ref);

	if (fail) {
		prthex(lab, data, len);
	}

	return fail;
}

//  boolean return value check
int chkret(const char* lab, int want, int have)
{
	printf("[%s] %s WANT=%d  HAVE=%d\n",
		want != have ? "FAIL" : "PASS", lab, want, have);

	return want != have ? 1 : 0;
}

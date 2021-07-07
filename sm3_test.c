#include "test_hex.h"
#include "sm3_wrap.h"

int test_sm3()
{
	/*
	* 使用指定数据进行SM3 测试
	*/
	uint8_t md[32], in[256];
	int fail = 0;

	sm3_256(md, "abc", 3);								// 使用a,b,c(61626300000...)进行测试
	fail += chkhex("SM3-256", md, 32,
				   "66C7F0F462EEEDD9D1F2D46BDC10E4E24167C4875CF2F7A2297DA02B8F4BA8E0");

	sm3_256(md, in, readhex(in, sizeof(in),
							"6162636461626364616263646162636461626364616263646162636461626364"
							"6162636461626364616263646162636461626364616263646162636461626364"));		// 使用a,b,c,d,a,b,c,d...(6162636461626364...)进行测试
	fail += chkhex("SM3-256", md, 32,
				   "DEBE9FF92275B8A138604889C18E5A4D6FDB70E5387E5765293DCBA39C0C5732");

	return fail;
}

/*
在测试主函数中使用下列语句进行SM3调用
int test_sm3();								//  sm3_test.c

int main(){
	printf("[INFO] === SM3 tests ===\n");
	fail += test_sm3();
}
*/
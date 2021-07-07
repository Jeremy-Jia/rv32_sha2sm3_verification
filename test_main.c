#include <stdio.h>

#include "sha2_wrap.h"
#include "sm3_wrap.h"


int test_sha2_256();						//  sha2_test.cpp
int test_sha2_512();
int test_sha2_hmac();

int test_sm3();								//  test_sm3.cpp

int main(int argc, char** argv)
{
	//// rv32b_clmulh函数测试
	char s[33];
	printf("******************\t[TEST] rv32b_clmul \t********************\n");

	uint32_t a3 = 0b01001100000000000000000000000000;
	a3 = 1275068416;
	uint32_t b3 = 0b11001000000000000000000000000000;
	uint32_t test_c3;
	printf("a3 is 0x%8x\t", a3);
	printf("b3 is 0x%8x\t", b3);
	test_c3 = rv32b_clmulh(a3, b3);
	printf("c3 is 0x%8x\n", test_c3);

	printf("a3 is 0x%8x\t", a3);
	printf("b3 is 0x%8x\t", b3);
	test_c3 = rv32b_clmulr(a3, b3);
	printf("c3 is 0x%8x\n", test_c3);

	printf("******************\t[TEST] rv32b_clmul \t********************\n\n");

	int fail = 0;
	//printf("[INFO] === sing")
	//uint8_t md[32];
	//sha2_256(md, "abc", 3);

	printf("[INFO] === SHA2-256 using rv32_sha256_compress() ===\n");
	sha256_compress = rv32_sha256_compress;
	fail += test_sha2_256();

	printf("\n\n\n[INFO] === SHA2-512 using rv32_sha512_compress() ===\n");
	sha512_compress = rv32_sha512_compress;
	fail += test_sha2_512();

	printf("\n\n\n[INFO] === rv32_sha256_compress() rv64_sha512_compress() ===\n");
	sha256_compress = rv32_sha256_compress;
	fail += test_sha2_hmac();

	
	printf("[INFO] === SM3 tests ===\n");
	fail += test_sm3();
	

	printf("\n[%s] === finished with %d unit test failures ===\n",
		fail == 0 ? "PASS" : "FAIL", fail);


	printf("********test sha2 sm3 and b extension*********\n");
  

	printf("********test sha256sig0*********\n");
	uint32_t a5 = 1, b5 = 0;
	b5 = sha256_sig0(a5);
	printf("b5 = %d\n", b5);


	printf("********test sha256sig1*********\n");
	uint32_t a6 = 1, b6 = 0;
	b6 = sha256_sig1(a6);
	printf("b6 = %d\n", b6);


	printf("********test sha256sum0*********\n");
	uint32_t a7 = 1, b7 = 0;
	b7 = sha256_sum0(a7);
	printf("b7 = %d\n", b7);

	printf("********test sha256sum1*********\n");
	uint32_t a8 = 1, b8 = 0;
	b8 = sha256_sum1(a8);
	printf("b8 = %d\n", b8);






	printf("********test sha512sig0h*********\n");
	uint32_t a9 = 1, b9 = 0;
	b9 = sha512_sig0h(a9,2);
	printf("b9 = %d\n", b9);



	printf("********test sha512_sig0l*********\n");
	uint32_t a10 = 1, b10 = 0;
	b10 = sha512_sig0l(a10,2);
	printf("b10 = %d\n", b10);



	printf("********test sha512_sig1h*********\n");
	uint32_t a11 = 1, b11 = 0;
	b11 = sha512_sig1h(1,2);
	printf("b11 = %d\n", b11);


	printf("********test sha512_sig1l*********\n");
	uint32_t a12 = 1, b12 = 0;
	b12 = sha512_sig1l(1,2);
	printf("b12 = %d\n", b12);




	printf("********test sha512_sum0r*********\n");
	uint32_t a13 = 1, b13 = 0;
	b13 = sha512_sum0r(a13,2);
	printf("b13 = %d\n", b13);



	printf("********test sha512_sum1r*********\n");
	uint32_t a14 = 1, b14 = 0;
	b14 = sha512_sum1r(a14,2);
	printf("b14 = %d\n", b14);

	printf("********test sm3p0*********\n");
	uint32_t a15 = 1, b15 = 0;
	b15 = sm3_p0(a15);
	printf("b15 = %d\n", b15);

	printf("********test sm3p1*********\n");
	uint32_t a16 = 1, b16 = 0;
	b16 = sm3_p1(a16);
	printf("b16 = %d\n", b16);





	printf("********test andn*********\n");
	uint32_t a17 = 1, b17 = 0;
	b17 = rv32b_andn(1,1);
	printf("b17 = %d\n", b17);


	printf("********test ror*********\n");
	uint32_t a18 = 1, b18 = 0;
	b18 = rv32b_ror(1,1);
	printf("b18 = %d\n", b18);


	printf("********test grev*********\n");
	uint32_t a19 = 1, b19 = 0;
	b19 = rv32b_grev(1, 1);
	printf("b19 = %d\n", b19);

	printf("********test clmul*********\n");
	uint32_t a20 = 1, b20 = 0;
	b20 = rv32b_clmul(1, 1);
	printf("b20 = %d\n", b20);


	printf("********test clmulh*********\n");
	uint32_t a21 = 1, b21= 0;
	b21 = rv32b_clmulh(1, 1);
	printf("b21 = %d\n", b21);


	printf("********test shfli*********\n");
	uint32_t a22 = 1, b22 = 0;
	b22 = rv32b_shfl(1, 1);
	printf("b22 = %d\n", b22);


	printf("********test unshfli*********\n");
	uint32_t a23 = 1, b23 = 0;
	b23 = rv32b_unshfl(1, 1);
	printf("b23 = %d\n", b23);


	printf("********test pack*********\n");
	uint32_t a24 = 1, b24 = 0;
	b24 = rv32b_pack(1, 1);
	printf("b24 = %d\n", b24);


	printf("********test packu*********\n");
	uint32_t a25 = 1, b25 = 0;
	b25 = rv32b_packu(1, 1);
	printf("b25 = %d\n", b25);

	printf("********test packh*********\n");
	uint32_t a26 = 1, b26 = 0;
	b26 = rv32b_packh(1, 1);
	printf("b26 = %d\n", b26);


	printf("********test gorc*********\n");
	uint32_t a27 = 1, b27 = 0;
	b27 = rv32b_gorc(1, 1);
	printf("b27 = %d\n", b27);


	return 0;
}
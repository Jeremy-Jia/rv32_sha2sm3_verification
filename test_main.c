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


	printf("********test grev7/24*********\n");
	uint32_t a19 = 1, b19_7 = 0,b19_24;
	b19_7 = rv32b_grev(1, 7);
	printf("b19_7 = %d\n", b19_7);
	b19_24 = rv32b_grev(1, 24);
	printf("b19_24 = %d\n", b19_24);

	printf("********test clmul*********\n");
	uint32_t a20 = 1, b20 = 0;
	b20 = rv32b_clmul(1, 1);
	printf("b20 = %d\n", b20);


	printf("********test clmulh*********\n");
	uint32_t a21 = 1, b21= 0;
	b21 = rv32b_clmulh(1, 1);
	printf("b21 = %d\n", b21);


	printf("********test shfli_15*********\n");
	uint32_t a22 = 1, b22 = 0;
	b22 = rv32b_shfl(1, 15);
	printf("b22 = %d\n", b22);


	printf("********test unshfli_15*********\n");
	uint32_t a23 = 1, b23 = 0;
	b23 = rv32b_unshfl(1, 15);
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


	printf("********test gorc3/4/7*********\n");
	uint32_t a27 = 1, b27_3 = 0,b27_4 = 0,b27_7 = 0;
	b27_3 = rv32b_gorc(1, 3);
	printf("b27_3 = %d\n", b27_3);
	b27_4 = rv32b_gorc(1, 4);
	printf("b27_4 = %d\n", b27_4);
	b27_7 = rv32b_gorc(1, 7);
	printf("b27_7 = %d\n", b27_7);


	printf("********test xperm_n*********\n");
	uint32_t a28 = 1, b28 = 0;
	b28 = rv32b_xperm_n(1, 1);
	printf("b28 = %d\n", b28);

	printf("********test xperm_b*********\n");
	uint32_t a29 = 1, b29 = 0;
	b29 = rv32b_xperm_b(1, 1);
	printf("b29 = %d\n", b29);

	printf("********test rori*********\n");
	uint32_t a30 = 1, b30 = 0;
	b30 = rv32b_ror(1, 1);
	printf("b30 = %d\n", b30);

	printf("********test rol*********\n");
	uint32_t a31 = 1, b31 = 0;
	b31 = rv32b_rol(1, 1);
	printf("b31 = %d\n", b31);


	printf("********test orn*********\n");
	uint32_t a32 = 1, b32 = 0;
	b32 = rv32b_orn(1, 1);
	printf("b32 = %d\n", b32);

	printf("********test xorn*********\n");
	uint32_t a33 = 1, b33 = 0;
	b33 = rv32b_xnor(1, 1);
	printf("b33 = %d\n", b33);



	return 0;
}

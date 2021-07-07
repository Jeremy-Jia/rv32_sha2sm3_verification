#ifndef _SHA2_WRAP_H_
#define _SHA2_WRAP_H_

#include <stddef.h>
#include <stdint.h>

// === 仅调用hash算法及其对应包装器 ===
// SHA2-224: Compute 28-byte hash to "md" from "in" which has "inlen" bytes.
// 输入为“in”（任意长度），长度为inlen（可调用readhex计算），输出为md（28个字节，224bits）
void sha2_224(uint8_t * md, const void *in, size_t inlen);

//  SHA2-256: Compute 32-byte hash to "md" from "in" which has "inlen" bytes.
// 输入为“in”（任意长度），长度为inlen（可调用readhex计算），输出为md（32个字节，256bits）
void sha2_256(uint8_t * md, const void *in, size_t inlen);

//  SHA2-384: Compute 48-byte hash to "md" from "in" which has "inlen" bytes.
// 输入为“in”（任意长度），长度为inlen（可调用readhex计算），输出为md（48个字节，384bits）
void sha2_384(uint8_t * md, const void *in, size_t inlen);

//  SHA2-512: Compute 64-byte hash to "md" from "in" which has "inlen" bytes.
// 输入为“in”（任意长度），长度为inlen（可调用readhex计算），输出为md（48个字节，512bits）
void sha2_512(uint8_t * md, const void *in, size_t inlen);


//  === HMAC wrappers ===
//  === 套用sha2系列算法的hmac生成器 ===
//  HMAC-SHA2-224: Compute 28-byte "mac" with key "k" of "klen" bytes.
// 输入消息为“in”（任意长度），长度为inlen（可调用readhex计算）
// 输入秘钥为“k”（256bits）
void hmac_sha2_224(uint8_t * mac, const void *k, size_t klen,
				   const void *in, size_t inlen);

//  HMAC-SHA2-256: Compute 32-byte "mac" with key "k" of "klen" bytes.
void hmac_sha2_256(uint8_t * mac, const void *k, size_t klen,
				   const void *in, size_t inlen);

//  HMAC-SHA2-384: Compute 48-byte "mac" with key "k" of "klen" bytes.
void hmac_sha2_384(uint8_t * mac, const void *k, size_t klen,
				   const void *in, size_t inlen);

//  HMAC-SHA2-512: Compute 64-byte "mac" with key "k" of "klen" bytes.
void hmac_sha2_512(uint8_t * mac, const void *k, size_t klen,
				   const void *in, size_t inlen);

//  === Compression Functions ===

//  function pointer to the compression function used by the test wrappers
extern void (*sha256_compress)(void *);
extern void (*sha512_compress)(void *);

//  SHA-256 compression function for RV32 (rv32_sha256.c)
void rv32_sha256_compress(void *s);

//  SHA-512 compression function for RV32 (rv32_sha512.c)
void rv32_sha512_compress(void *s);

#endif
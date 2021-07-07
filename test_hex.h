#ifndef _TEST_HEX_H_
#define _TEST_HEX_H_

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

//  read a hex string of "maxbytes", return byte length
// 读取str字符串，规定最大长度为maxbytes，返回字符串长度，将字符串整形至buf空间中
size_t readhex(uint8_t * buf, size_t maxbytes, const char *str);

//  print hexadecimal "data", length "len", with label "lab"
// 
void prthex(const char *lab, const void *data, size_t len);

//  check "data" of "len" bytes against a hexadecimal test vector "ref"
int chkhex(const char *lab, const void *data, size_t len, const char *ref);

//  boolean return value check
int chkret(const char *lab, int want, int have);

#endif
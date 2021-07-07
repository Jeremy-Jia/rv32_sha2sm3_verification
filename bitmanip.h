#ifndef _BITMANIP_H_
#define _BITMANIP_H_

#include <stdint.h>
 //total 17 instructions except rv32b_clmulr rv32b_xperm_h rv32b_xperm_w

uint32_t rv32b_ror(uint32_t rs1, uint32_t rs2);
uint32_t rv32b_rol(uint32_t rs1, uint32_t rs2);

//  and with negate ANDN 
uint64_t rv32b_andn(uint32_t rs1, uint32_t rs2);
uint32_t rv32b_orn(uint32_t rs1, uint32_t rs2);
uint32_t rv32b_xnor(uint32_t rs1, uint32_t rs2);


uint32_t rv32b_gorc(uint32_t rs1, uint32_t rs2);

uint32_t rv32b_pack(uint32_t rs1, uint32_t rs2);
uint32_t rv32b_packu(uint32_t rs1, uint32_t rs2);
uint32_t rv32b_packh(uint32_t rs1, uint32_t rs2);

//  generalized reverse GREV / GREVI 
uint32_t rv32b_grev(uint32_t rs1, uint32_t rs2);

//  generalized shuffle SHFL / SHFLI
uint32_t rv32b_shfl(uint32_t rs1, uint32_t rs2);

//  generalized unshuffle UNSHFL / UNSHFLI
uint32_t rv32b_unshfl(uint32_t rs1, uint32_t rs2);

//  carryless multiply 
uint32_t rv32b_clmul(uint32_t rs1, uint32_t rs2);
uint32_t rv32b_clmulh(uint32_t rs1, uint32_t rs2);
uint32_t rv32b_clmulr(uint32_t rs1, uint32_t rs2);
 /*
uint32_t rv32b_xperm_n(uint32_t rs1, uint32_t rs2)
uint32_t rv32b_xperm_b(uint32_t rs1, uint32_t rs2)
uint32_t rv32b_xperm_h(uint32_t rs1, uint32_t rs2)
uint32_t rv32b_xperm_w(uint32_t rs1, uint32_t rs2)
 */

#endif										
//  _BITMANIP_H_
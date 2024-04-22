#ifndef DIFFIE_HELLMAN_H
#define DIFFIE_HELLMAN_H

#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>
#include "string.h"
#include "uart.h"

#define data_size 6

void set_zero(uint32_t* a, uint8_t size);
void set_rand(uint32_t* a, uint8_t size);
void print(uint32_t* a, uint8_t size);
uint32_t less(uint32_t* a, uint32_t* b, uint8_t size);
void op_minus(uint32_t* a, uint32_t* b, uint8_t size);
void op_plus(uint32_t* a, uint32_t* b, uint8_t size);
uint32_t zero(uint32_t* a, uint8_t size);
void cpy(uint32_t* a, uint32_t* b, uint8_t size);
void shift_l1(uint32_t* a, uint8_t size);
void shift_r1(uint32_t* a, uint8_t size);
void mul_bit(uint32_t* a, uint32_t* b, uint8_t size);
void mod_bit(uint32_t* a, uint32_t* m, uint8_t size);
void pow_mod_bit(uint32_t* a, uint32_t* b, uint32_t* m, uint8_t size);
void set_key(HANDLE port, uint32_t* k);

#endif // !DIFFIE_HELLMAN_H

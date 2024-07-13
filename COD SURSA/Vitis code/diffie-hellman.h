#ifndef DIFFIE_HELLMAN_H
#define DIFFIE_HELLMAN_H
#include "stdint.h"
#include <stdint.h>

#define data_size 6

void set_zero(uint32_t *a, uint8_t size);
void set_rand(uint32_t *a, uint8_t size);
void send_as_hex(uint32_t *a, char *str, uint8_t size);
uint32_t is_less(uint32_t *a, uint32_t *b, uint8_t size);
void subtraction(uint32_t *a, uint32_t *b, uint8_t size);
void addition(uint32_t *a, uint32_t *b, uint8_t size);
uint32_t is_zero(uint32_t *a, uint8_t size);
void cpy(uint32_t *a, uint32_t *b, uint8_t size);
void shift_left(uint32_t *a, uint8_t size);
void shift_right(uint32_t *a, uint8_t size);
void multiply(uint32_t *a,uint32_t*b, uint8_t size);
void modulo(uint32_t *a,uint32_t*m, uint8_t size);
void pow_in_modulo(uint32_t *a,uint32_t *b,uint32_t *m, uint8_t size);
void test_local_key();

#endif
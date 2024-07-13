#ifndef DIFFIE_HELLMAN_H
#define DIFFIE_HELLMAN_H

#include <stdio.h> 
#include "string.h"
#include "uart.h"

#define data_size 6
#define xil_printf(fmt, ...) recv_index += sprintf(&recv_buff[recv_index],fmt, ##__VA_ARGS__)

extern char* recv_buff;
extern int recv_index;
extern char out_op;

void set_zero(uint32_t* a, uint8_t size);
void set_rand(uint32_t* a, uint8_t size);
void print_as_hex(uint32_t* a, uint8_t size);
void print_as_char(uint32_t* a, uint8_t size);
void switch_prints(uint32_t* a, uint8_t size);
uint32_t is_less(uint32_t* a, uint32_t* b, uint8_t size);
void subtraction(uint32_t* a, uint32_t* b, uint8_t size);
void addition(uint32_t* a, uint32_t* b, uint8_t size);
uint32_t is_zero(uint32_t* a, uint8_t size);
void cpy(uint32_t* a, uint32_t* b, uint8_t size);
void shift_left(uint32_t* a, uint8_t size);
void shift_right(uint32_t* a, uint8_t size);
void multiply(uint32_t* a, uint32_t* b, uint8_t size);
void modulo(uint32_t* a, uint32_t* m, uint8_t size);
void pow_in_modulo(uint32_t* a, uint32_t* b, uint32_t* m, uint8_t size);
void key_agreement(HANDLE port, uint32_t* k);

#endif // !DIFFIE_HELLMAN_H

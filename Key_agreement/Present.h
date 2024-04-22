#ifndef PRESENT_H
#define PRESENT_H

#include "diffie-hellman.h"

uint32_t S(uint32_t w, uint32_t offset);
void bit_set(uint32_t* w, uint8_t val, uint8_t pos);
uint8_t get_bit(uint32_t* aux, uint8_t i);
void update_word(uint32_t* word, uint32_t* key, uint32_t counter);
void update_key(uint32_t* key, uint32_t counter);
void unupdate_key(uint32_t* key, uint32_t counter);
void encript(uint32_t* text, uint32_t* key);
void decript(uint32_t* text, uint32_t* key);
void test_Present();

#endif
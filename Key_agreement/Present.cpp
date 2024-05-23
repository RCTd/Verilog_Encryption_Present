#include "Present.h"

uint32_t S(uint32_t w, uint32_t offset)
{
    uint32_t mask = (0xF << 4 * offset);
    uint32_t cleared_number = w & ~mask;
    uint32_t value = (w ^ cleared_number) >> (4 * offset);
    switch (value)
    {
    case 0x0: {value = 0xc; break; }
    case 0x1: {value = 0x5; break; }
    case 0x2: {value = 0x6; break; }
    case 0x3: {value = 0xb; break; }
    case 0x4: {value = 0x9; break; }
    case 0x5: {value = 0x0; break; }
    case 0x6: {value = 0xa; break; }
    case 0x7: {value = 0xd; break; }
    case 0x8: {value = 0x3; break; }
    case 0x9: {value = 0xe; break; }
    case 0xa: {value = 0xf; break; }
    case 0xb: {value = 0x8; break; }
    case 0xc: {value = 0x4; break; }
    case 0xd: {value = 0x7; break; }
    case 0xe: {value = 0x1; break; }
    case 0xf: {value = 0x2; break; }
    default:
        break;
    }
    value = value << (4 * offset);
    return value | cleared_number;
}
uint32_t S1(uint32_t w, uint32_t offset)
{
    uint32_t mask = (0xF << 4 * offset);
    uint32_t cleared_number = w & ~mask;
    uint32_t value = (w ^ cleared_number) >> (4 * offset);
    switch (value)
    {
    case 0xc: {value = 0x0; break; }
    case 0x5: {value = 0x1; break; }
    case 0x6: {value = 0x2; break; }
    case 0xb: {value = 0x3; break; }
    case 0x9: {value = 0x4; break; }
    case 0x0: {value = 0x5; break; }
    case 0xa: {value = 0x6; break; }
    case 0xd: {value = 0x7; break; }
    case 0x3: {value = 0x8; break; }
    case 0xe: {value = 0x9; break; }
    case 0xf: {value = 0xa; break; }
    case 0x8: {value = 0xb; break; }
    case 0x4: {value = 0xc; break; }
    case 0x7: {value = 0xd; break; }
    case 0x1: {value = 0xe; break; }
    case 0x2: {value = 0xf; break; }
    default:
        break;
    }
    value = value << (4 * offset);
    return value | cleared_number;
}

void bit_set(uint32_t* w, uint8_t val, uint8_t pos) {
    w[0] = (w[0] & ~(1 << pos)) | (val << pos);
}

uint8_t get_bit(uint32_t* aux, uint8_t i) {
    return (aux[i / 32] >> (i % 32)) & 1;
}

void update_word(uint32_t* word, uint32_t* key, uint32_t counter)
{
    uint32_t aux[3];
    memcpy(aux, key, 3 * sizeof(uint32_t));
    //printf("\nword%d ", counter);
    //print(word);

    //printf("\nX ");
    for (int i = 0; i < 16; i++) shift_r1(aux,3);
    for (int i = 0; i < 2; i++) {
        word[i] = word[i] ^ aux[i];
    }
    //print(word);

    //printf("\nS ");
    for (int i = 0; i < 16; i++) {
        word[i / 8] = S(word[i / 8], i % 8);
    }
    //print(word);

    //printf("\nP ");
    memcpy(aux, word, 2 * sizeof(uint32_t));
    for (uint8_t i = 0; i < 64; i++) {
        bit_set(&word[i / 32], get_bit(aux, i / 16 + 4 * (i % 16)), i % 32);
    }
    //print(word);
}


void update_key(uint32_t* key, uint32_t counter)
{
    uint32_t aux[3];
    memcpy(aux, key, 3 * sizeof(uint32_t));
    /*printf("\n\tkey%d ", counter);
    print(key);*/

    //printf("\n\tP ");
    for (uint8_t i = 0; i < 80; i++)
        bit_set(&key[i / 32], get_bit(aux, (i + 19) % 80), i % 32);
    //print(key);

    //printf("\n\tS ");
    key[2] = S(key[2], 3);
    //print(key);

    //printf("\n\tX ");
    key[0] ^= counter << 15;
    //print(key);
}

void encript(uint32_t* text, uint32_t* key)
{
    for (uint32_t counter = 0; counter < 31; counter++)
    {
        update_word(text, key, counter);
        update_key(key, counter + 1);
    }
    uint32_t aux[3];
    memcpy(aux, key, 3 * sizeof(uint32_t));
    for (int i = 0; i < 16; i++) shift_r1(aux,3);
    for (int i = 0; i < 2; i++) {
        text[i] = text[i] ^ aux[i];
    }
}

void unupdate_word(uint32_t* word, uint32_t* key, uint32_t counter)
{
    uint32_t aux[3];
    //printf("\nP ");
    memcpy(aux, word, 2 * sizeof(uint32_t));
    for (uint8_t i = 0; i < 64; i++) {
        bit_set(&word[i / 32], get_bit(aux, (i%4)*16+i/4), i % 32);
    }
    //print(word);

    //printf("\nS ");
    for (int i = 0; i < 16; i++) {
        word[i / 8] = S1(word[i / 8], i % 8);
    }
    //print(word);

    memcpy(aux, key, 3 * sizeof(uint32_t));
    //printf("\nX ");
    for (int i = 0; i < 16; i++) shift_r1(aux,3);
    for (int i = 0; i < 2; i++) {
        word[i] = word[i] ^ aux[i];
    }
    //print(word);
}

void unupdate_key(uint32_t* key, uint32_t counter)
{
    uint32_t aux[3];
    //printf("\n\tX ");
    //print(key);

    //printf("\n\tS ");
    key[0] ^= counter << 15;
    //print(key);

    //printf("\n\tP ");
    key[2] = S1(key[2], 3);
    //print(key);

    memcpy(aux, key, 3 * sizeof(uint32_t));
    //printf("\n\tkey%d ", counter);
    for (uint8_t i = 0; i < 80; i++)
        bit_set(&key[i / 32], get_bit(aux, (i +61) % 80), i % 32);
    //print(key);
}

void decript(uint32_t* text, uint32_t* key)
{
    for (uint32_t counter = 0; counter < 31; counter++)
    {
        update_key(key, counter + 1);
    }
    uint32_t aux[3];
    memcpy(aux, key, 3 * sizeof(uint32_t));
    for (int i = 0; i < 16; i++) shift_r1(aux,3);
    for (int i = 0; i < 2; i++) {
        text[i] = text[i] ^ aux[i];
    }
    memcpy(aux, key, 3 * sizeof(uint32_t));
    for (char counter = 31; counter > 0; counter--)
    {
        unupdate_key(key, counter);
        unupdate_word(text, key, counter);
    }
    memcpy(key, aux, 3 * sizeof(uint32_t));
}

void test_Present()
{
    uint32_t text[2];
    set_zero(text, 2);
    
    uint32_t key[3];
    set_zero(key,3);

    uint32_t key2[3];
    set_zero(key2,3);

    for (int i = 0; i < 5; i++) {
        printf("Encript\n");
        encript(text, key2);

        printf("text\n");
        print(text,2);
        printf("key\n");
        print(key2,3);
        //....................
        printf("Decript\n");
        decript(text, key);

        printf("text\n");
        print(text,2);
        printf("key\n");
        print(key,3);
    }
}
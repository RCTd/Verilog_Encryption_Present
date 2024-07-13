#include "pch.h"

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
uint32_t inv_S(uint32_t w, uint32_t offset)
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

void set_bit(uint32_t* w, uint8_t val, uint8_t pos) {
    w[0] = (w[0] & ~(1 << pos)) | (val << pos);
}

uint8_t get_bit(uint32_t* aux, uint8_t i) {
    return (aux[i / 32] >> (i % 32)) & 1;
}

void update_word(uint32_t* word, uint32_t* key, uint32_t counter)
{
    uint32_t aux[3];
    memcpy(aux, key, 3 * sizeof(uint32_t));
    //<<61
    for (int i = 0; i < 16; i++) shift_right(aux, 3);
    //xor key
    for (int i = 0; i < 2; i++) {
        word[i] = word[i] ^ aux[i];
    }
    //S
    for (int i = 0; i < 16; i++) {
        word[i / 8] = S(word[i / 8], i % 8);
    }
    memcpy(aux, word, 2 * sizeof(uint32_t));
    //P
    for (uint8_t i = 0; i < 64; i++) {
        set_bit(&word[i / 32], get_bit(aux, i / 16 + 4 * (i % 16)), i % 32);
    }
}

void update_key(uint32_t* key, uint32_t counter)
{
    uint32_t aux[3];
    memcpy(aux, key, 3 * sizeof(uint32_t));
    //P
    for (uint8_t i = 0; i < 80; i++)
        set_bit(&key[i / 32], get_bit(aux, (i + 19) % 80), i % 32);
    //s
    key[2] = S(key[2], 3);
    //xor counter
    key[0] ^= counter << 15;
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
    for (int i = 0; i < 16; i++) shift_right(aux, 3);
    for (int i = 0; i < 2; i++) {
        text[i] = text[i] ^ aux[i];
    }
}

void reverse_word(uint32_t* word, uint32_t* key, uint32_t counter)
{
    uint32_t aux[3];
    memcpy(aux, word, 2 * sizeof(uint32_t));
    //P
    for (uint8_t i = 0; i < 64; i++) {
        set_bit(&word[i / 32], get_bit(aux, (i % 4) * 16 + i / 4), i % 32);
    }
    //S
    for (int i = 0; i < 16; i++) {
        word[i / 8] = inv_S(word[i / 8], i % 8);
    }
    memcpy(aux, key, 3 * sizeof(uint32_t));
    //key << 61
    for (int i = 0; i < 16; i++) shift_right(aux, 3);
    //xor key
    for (int i = 0; i < 2; i++) {
        word[i] = word[i] ^ aux[i];
    }
}

void reverse_key(uint32_t* key, uint32_t counter)
{
    uint32_t aux[3];
    //xor counter
    key[0] ^= counter << 15;
    //S
    key[2] = inv_S(key[2], 3);
    memcpy(aux, key, 3 * sizeof(uint32_t));
    //P
    for (uint8_t i = 0; i < 80; i++)
        set_bit(&key[i / 32], get_bit(aux, (i + 61) % 80), i % 32);
}

void decript(uint32_t* text, uint32_t* key)
{
    for (uint32_t counter = 0; counter < 31; counter++)
    {
        update_key(key, counter + 1);
    }
    uint32_t aux[3];
    memcpy(aux, key, 3 * sizeof(uint32_t));
    for (int i = 0; i < 16; i++) shift_right(aux, 3);
    for (int i = 0; i < 2; i++) {
        text[i] = text[i] ^ aux[i];
    }
    memcpy(aux, key, 3 * sizeof(uint32_t));
    for (char counter = 31; counter > 0; counter--)
    {
        reverse_key(key, counter);
        reverse_word(text, key, counter);
    }
    memcpy(key, aux, 3 * sizeof(uint32_t));
}

uint32_t hexCharToInt(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    else if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else
        return 0;  // Invalid character (shouldn't happen if input is valid hex)
}

void recive_data(HANDLE port, char c, uint32_t* k)
{
    char buff[99];
    int i = 0, j = 0, b = 0;
    uint32_t result[2];
    set_zero(result, 2);
    uint32_t aux = 0;
    do
    {
        i = read_port(port, (uint8_t*)buff, 1);
        if (buff[0] != 0 && i > 0)
        {
            for (int i = 0; i < 4; i++)
                shift_left(result, 2);
            result[0] = result[0] | hexCharToInt(buff[0]);
            j++;
            b++;
            if (j >= 2) {
                j = 0;
                if (result[0] == 4) {
                    return;
                }
            }
            if (b >= 16)
            {
                b = 0;
                switch (c)
                {
                case 'N':
                    switch_prints(result, 2);
                    break;
                case 'E':
                    encript(result, k);
                    switch_prints(result, 2);
                    break;
                case 'D':
                    decript(result, k);
                    aux = result[0];
                    result[0] = result[1];
                    result[1] = aux;
                    switch_prints(result, 2);
                    break;
                default:
                    break;
                }
                set_zero(result, 2);
            }

        }
    } while (1);
}

void send_data(char* str, HANDLE port, char c) {
    size_t len = strlen(str);
    size_t i;
    size_t num_values = len;
    uint8_t array[8];
    array[0] = c;
    while (write_port(port, array, 1));
    for (i = 0; i < num_values || i % 8; i++) {
        if (i < num_values)
            array[i % 8] = ((unsigned char)str[i]);
        else
            array[i % 8] = 0;

        if (i % 8 == 7) {
            std::this_thread::sleep_for(std::chrono::milliseconds(12));
            write_port(port, array, 8);
        }
    }
    for (int i = 0; i < 7; i++) { array[i] = 0; }
    array[7] = 4;
    write_port(port, array, 8);
}
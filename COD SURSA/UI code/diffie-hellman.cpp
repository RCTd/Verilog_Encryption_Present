#include "pch.h"

#include "diffie-hellman.h"

void swap_8(uint8_t* a, uint8_t size)
{
    uint8_t aux;
    for (int i = 0; i < size / 2; i++) {
        aux = a[i];
        a[i] = a[size - 1 - i];
        a[size - 1 - i] = aux;
    }
}

void swap_32(uint32_t* a, uint8_t size)
{
    for (int i = 0; i < size; i++)
    {
        swap_8((uint8_t*)(a + i), 4);
    }
    uint32_t aux;
    for (int i = 0; i < size / 2; i++) {
        aux = a[i];
        a[i] = a[size - 1 - i];
        a[size - 1 - i] = aux;
    }
}

void print_as_hex(uint32_t* a, uint8_t size)
{
    for (int i = size - 1; i >= 0; i--) {
        xil_printf("%08X", a[i]);
    }
    xil_printf("\r\n");
}

void print_as_char(uint32_t* a, uint8_t size)
{
    for (int i = size - 1; i >= 0; i--) {
        for (int j = sizeof(uint32_t) - 1; j >= 0; j--)
        {
            xil_printf("%c", (char)((a[i] & (0xff << (j * 8))) >> (j * 8)));
        }
    }
}


void switch_prints(uint32_t* a, uint8_t size)
{
    switch (out_op)
    {
    case 'H':
        print_as_hex(a, size);
        break;
    case 'S':
        print_as_char(a, size);
        break;
    default:
        break;
    }
}

void set_zero(uint32_t* a, uint8_t size)
{
    for (int i = 0; i < size; i++) { a[i] = 0; }
}
void set_rand(uint32_t* a, uint8_t size)
{
    set_zero(a, size);
    for (int i = 0; i < size / 2; i++) { a[i] = (uint32_t)rand(); }
}

void send_as_hex(uint32_t* a, char* str, uint8_t size)
{
    if (str)
        xil_printf("%s\r\n", str);
    for (int i = size - 1; i >= 0; i--) {
        xil_printf("%08x", a[i]);
    }
    if (str)
        xil_printf("\r\n");
}

uint32_t is_less(uint32_t* a, uint32_t* b, uint8_t size)
{
    for (int i = size - 1; i >= 0; i--)
    {
        if (a[i] > b[i]) {
            return 0;
        }
        if (a[i] < b[i]) {
            return 1;
        }
    }
    return 0;
}

void subtraction(uint32_t* a, uint32_t* b, uint8_t size)
{
    for (int i = 0; i < size; i++) {
        if (a[i] < b[i]) {
            for (int j = i + 1; j < size; j++) {
                a[j]--;
                if (a[j] + 1 > 0) {
                    j = size;
                }
            }
        }
        a[i] -= b[i];
    }
}

void addition(uint32_t* a, uint32_t* b, uint8_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (a[i] > UINT32_MAX - b[i])
        {
            for (int j = i + 1; j < size; j++)
            {
                a[j]++;
                if (a[j] - 1 < UINT32_MAX)
                {
                    j = size;
                }
            }
        }
        a[i] += b[i];
    }
}

uint32_t is_zero(uint32_t* a, uint8_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (a[i])
        {
            return 0;
        }
    }
    return 1;
}


void cpy(uint32_t* a, uint32_t* b, uint8_t size)
{
    memcpy(a, b, size * sizeof(uint32_t));
}

void shift_left(uint32_t* a, uint8_t size)
{
    for (int i = size - 1; i > 0; i--)
    {
        a[i] = a[i] << 1 | a[i - 1] >> (32 - 1);
    }
    a[0] = a[0] << 1;
}

void shift_right(uint32_t* a, uint8_t size)
{
    for (int i = 0; i < size - 1; i++)
    {
        a[i] = a[i] >> 1 | a[i + 1] << (32 - 1);
    }
    a[size - 1] = a[size - 1] >> 1;
}

void multiply(uint32_t* a, uint32_t* b, uint8_t size)
{
    uint32_t* ml = (uint32_t*)malloc(size * sizeof(uint32_t));
    uint32_t* mr = (uint32_t*)malloc(size * sizeof(uint32_t));
    cpy(ml, a, size);
    cpy(mr, b, size);
    set_zero(a, size);
    while (!is_zero(mr, size))
    {
        // print(mr, "mr");
        if (mr[0] & 1) { addition(a, ml, size); }
        shift_left(ml, size);
        shift_right(mr, size);
    }
    free(ml);
    free(mr);
}

void modulo(uint32_t* a, uint32_t* m, uint8_t size)
{
    uint32_t* div = (uint32_t*)malloc(size * sizeof(uint32_t));
    set_zero(div, size);
    memcpy(div + size / 2, m, size / 2 * sizeof(uint32_t));
    for (int i = 0; i <= size / 2 * 32; i++)
    {
        if (!is_less(a, div, size)) {
            subtraction(a, div, size);
        }
        shift_right(div, size);
    }
    free(div);
}

void pow_in_modulo(uint32_t* a, uint32_t* b, uint32_t* m, uint8_t size)
{
    uint32_t* mr = (uint32_t*)malloc(size * sizeof(uint32_t));
    uint32_t* aux = (uint32_t*)malloc(size * sizeof(uint32_t));
    cpy(mr, b, size);
    cpy(aux, a, size);
    modulo(aux, m, size);
    set_zero(a, size);
    a[0] = 1;
    while (!is_zero(mr, size))
    {
        if (mr[0] & 1)
        {
            multiply(a, aux, size);
            modulo(a, m, size);
        }
        multiply(aux, aux, size);
        modulo(aux, m, size);
        shift_right(mr, size);
    }
    free(mr);
    free(aux);
}

void key_agreement(HANDLE port, uint32_t* k)
{
    uint8_t size = 6;
    uint32_t b[data_size];
    set_rand(b, size);
    uint32_t g[data_size];
    uint32_t n[data_size];
    set_zero(g, size);
    set_zero(n, size);
    g[0] = 2;
    n[2] = 0x10000;
    n[0] = 0xd;
    pow_in_modulo(g, b, n, size);
    //printf("\nB\n");
    recv_index+=sprintf(&recv_buff[recv_index], "\nStart key agreement.\nSend to fpga: B\n");
    print_as_hex(g, size / 2);
    uint8_t buff[99];

    buff[0] = 'K';
    while (write_port(port, buff, 1)) {
        //printf("\nsend k");
        recv_index += sprintf(&recv_buff[recv_index], "\nsend k");
    }

    int i = 0;
    while (i < 29) //A
    {
        i += read_port(port, buff + i, 1);
    }
    buff[i] = '\0';
    //printf("\n%s", buff);
    recv_index += sprintf(&recv_buff[recv_index], "Recived: %s", buff);

    swap_32(g, 3);
    write_port(port, (uint8_t*)g, 24);
    swap_32(g, 3);

    i = 0;
    while (buff[i] != 'A') i++;
    memcpy(buff, buff + i + 3, 24);
    buff[24] = '\0';

    set_zero(g, size / 2);
    for (int i = 0; i < 3; i++) {
        char hexSegment[9];
        strncpy_s(hexSegment, (char*)(buff)+i * 8, 8);
        hexSegment[8] = '\0';
        g[2 - i] = (uint32_t)strtoul(hexSegment, NULL, 16);
    }
    pow_in_modulo(g, b, n, size);
    recv_index += sprintf(&recv_buff[recv_index], "Secret Key calculated:\n");
    print_as_hex(g, size / 2);

    cpy(k, g, size);
}
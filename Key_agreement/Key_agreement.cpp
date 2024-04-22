#include "diffie-hellman.h"

void set_zero(uint32_t* a, uint8_t size)
{
    for (int i = 0; i < size; i++) { a[i] = 0; }
}
void set_rand(uint32_t* a, uint8_t size)
{
    set_zero(a, size);
    for (int i = 0; i < size / 2; i++) { a[i] = (uint32_t)rand(); }
}

void print(uint32_t* a, uint8_t size)
{
    //printf("%s\r\n", str);
    for (int i = size - 1; i >= 0; i--) {
        printf("%08X", a[i]);
    }
    printf("\r\n");
}

uint32_t less(uint32_t* a, uint32_t* b, uint8_t size)
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

void op_minus(uint32_t* a, uint32_t* b, uint8_t size)
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

void op_plus(uint32_t* a, uint32_t* b, uint8_t size)
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

uint32_t zero(uint32_t* a, uint8_t size)
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

void shift_l1(uint32_t* a, uint8_t size)
{
    for (int i = size - 1; i > 0; i--)
    {
        a[i] = a[i] << 1 | a[i - 1] >> (32 - 1);
    }
    a[0] = a[0] << 1;
}

void shift_r1(uint32_t* a, uint8_t size)
{
    // xil_printf("shr\r\n");
    // print(a);
    for (int i = 0; i < size - 1; i++)
    {
        a[i] = a[i] >> 1 | a[i + 1] << (32 - 1);
    }
    a[size - 1] = a[size - 1] >> 1;
    // print(a);
}

void mul_bit(uint32_t* a, uint32_t* b, uint8_t size)
{
    // xil_printf("start mul bit \r\n");
    uint32_t *ml = (uint32_t*)malloc(size * sizeof(uint32_t));
    uint32_t *mr = (uint32_t*)malloc(size * sizeof(uint32_t));
    cpy(ml, a, size);
    cpy(mr, b, size);
    set_zero(a, size);
    while (!zero(mr, size))
    {
        // print(mr, "mr");
        if (mr[0] & 1) { op_plus(a, ml, size); }
        shift_l1(ml, size);
        shift_r1(mr, size);
    }
}

void mod_bit(uint32_t* a, uint32_t* m, uint8_t size)
{
    uint32_t *div = (uint32_t*)malloc(size * sizeof(uint32_t));
    set_zero(div, size);
    memcpy(div + size / 2, m, size / 2 * sizeof(uint32_t));
    for (int i = 0; i <= size / 2 * 32; i++)
    {
        if (!less(a, div, size)) {
            op_minus(a, div, size);
        }
        shift_r1(div, size);
    }
}

void pow_mod_bit(uint32_t* a, uint32_t* b, uint32_t* m, uint8_t size)
{
    uint32_t *mr = (uint32_t*)malloc(size * sizeof(uint32_t));
    uint32_t *aux = (uint32_t*)malloc(size * sizeof(uint32_t));
    cpy(mr, b, size);
    cpy(aux, a, size);
    mod_bit(aux, m, size);
    set_zero(a, size);
    a[0] = 1;
    while (!zero(mr, size))
    {
        if (mr[0] & 1)
        {
            mul_bit(a, aux, size);
            mod_bit(a, m, size);
        }
        mul_bit(aux, aux, size);
        mod_bit(aux, m, size);
        shift_r1(mr, size);
    }
}

void test_key_agr()
{
    printf("Hello world!\r\n");

    uint8_t size = 6;
    uint32_t *a = (uint32_t*)malloc(size * sizeof(uint32_t));
    uint32_t *b = (uint32_t*)malloc(size * sizeof(uint32_t));
    uint32_t *g = (uint32_t*)malloc(size * sizeof(uint32_t));
    uint32_t *n = (uint32_t*)malloc(size * sizeof(uint32_t));
    set_zero(g,size);
    set_zero(n,size);
    g[0] = 2;
    n[2] = 0x10000;
    n[0] = 0xd;

    uint32_t *g2 = (uint32_t*)malloc(size * sizeof(uint32_t));
    uint32_t *n2 = (uint32_t*)malloc(size * sizeof(uint32_t));
    set_zero(g2,size);
    set_zero(n2,size);
    g2[0] = 2;
    n2[2] = 0x10000;
    n2[0] = 0xd;

    set_rand(a,size);
    set_rand(b,size);

    print(a, size/2);
    pow_mod_bit(g, a, n,size);
    print(g, size);

    print(b,size/2);
    pow_mod_bit(g2, b, n2,size);
    print(g2,size);

    pow_mod_bit(g2, a, n,size);
    print(g2,size/2);
    pow_mod_bit(g, b, n2,size);
    print(g,size/2);

    printf("done!\r\n");
}

void set_key(HANDLE port,uint32_t *k)
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
    pow_mod_bit(g, b, n, size);
    printf("\nB\n");
    print(g, size/2);
    uint8_t buff[99];

    buff[0] = 'K';
    while (write_port(port, buff, 1));

    int i = 0;
    while (i < 29) //A
    {
        i += read_port(port, buff + i, 1);
    }
    buff[i] = '\0';
    printf("\n%s", buff);

    write_port(port, (uint8_t*)g, 24);
    print(g, size/2);

    i = 0;
    while (buff[i] != 'A') i++;
    memcpy(buff, buff + i + 3, 24);
    buff[24] = '\0';

    set_zero(g,size/2);
    for (int i = 0; i < 3; i++) {
        char hexSegment[9];
        strncpy_s(hexSegment, (char*)(buff)+i * 8, 8);
        hexSegment[8] = '\0';
        g[2 - i] = (uint32_t)strtoul(hexSegment, NULL, 16);
    }
    printf("my_a:");
    print(g,6/2);
    pow_mod_bit(g, b, n, size);
    printf("k:\n");
    print(g, size/2);

    i = 0;
    while (i < 29)
    {
        i += read_port(port, buff + i, 1);
    }
    buff[i] = '\0';
    printf("%s", buff);

    /*while (1)
    {
        i=read_port(port, buff, 1);
        if (buff[0] != 0 && i>0)
        {
            buff[1] = '\0';
            printf("%s", buff);
        }
    }*/

    cpy(k, g, size);
}
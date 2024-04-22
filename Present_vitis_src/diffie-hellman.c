#include "diffie-hellman.h"
#include <stdint.h>

void set_zero(uint32_t *a, uint8_t size)
{
    for(int i = 0; i < size; i++) {a[i] = 0;}
}
void set_rand(uint32_t *a,uint8_t size)
{
    set_zero(a,size);
    for(int i = 0; i < size/2; i++) {a[i] = (uint32_t) rand();}
}

void print(uint32_t *a, char *str, uint8_t size)
{
    if(str)
        xil_printf("%s\r\n",str);
    for (int i = size - 1; i >= 0; i--) {
        xil_printf("%08x",a[i]);
    }
    if(str)
        xil_printf("\r\n");
}

uint32_t less(uint32_t *a, uint32_t *b, uint8_t size)
{
    for(int i = size - 1; i >= 0; i--)
    {
        if(a[i] > b[i]){
            return 0;
        }
        if(a[i] < b[i]){
            return 1;
        }
    }
    return 0;
}

void op_minus(uint32_t *a, uint32_t *b, uint8_t size)
{
    for(int i = 0; i < size; i++){
        if(a[i]<b[i]){
            for(int j=i+1; j<size; j++){
                a[j]--;
                if(a[j]+1>0){
                    j=size;
                }
            }
        }
        a[i]-=b[i];
    }
}

void op_plus(uint32_t *a, uint32_t *b, uint8_t size)
{
    for(int i = 0; i < size; i++)
    {
        if(a[i] > UINT32_MAX - b[i])
        {
            for(int j=i+1; j<size; j++)
            {
                a[j]++;
                if(a[j]-1 < UINT32_MAX)
                {
                    j=size;
                }
            }
        }
        a[i]+=b[i];
    }
}

uint32_t zero(uint32_t *a, uint8_t size)
{
    for(int i = 0; i < size; i++)
    {
        if(a[i])
        {
            return 0;
        }
    }
    return 1;
}


void cpy(uint32_t *a, uint32_t *b, uint8_t size)
{
    memcpy(a ,b, size * sizeof(uint32_t));
}

void shift_l1(uint32_t *a, uint8_t size)
{
    for(int i = size-1; i > 0; i--)
    {
        a[i] = a[i] << 1 | a[i-1]>>(32-1);
    }
    a[0] = a[0] << 1;
}

void shift_r1(uint32_t *a, uint8_t size)
{
    // xil_printf("shr\r\n");
    // print(a);
    for(int i = 0; i < size-1; i++)
    {
        a[i] = a[i] >> 1 | a[i+1]<<(32-1);
    }
    a[size-1] = a[size-1] >> 1;
    // print(a);
}

void mul_bit(uint32_t *a,uint32_t*b, uint8_t size)
{
    // xil_printf("start mul bit \r\n");
    uint32_t ml[size];
    uint32_t mr[size];
    cpy(ml, a, size);
    cpy(mr, b, size);
    set_zero(a, size);
    while(!zero(mr, size))
    {
        // print(mr, "mr");
        if(mr[0] & 1) {op_plus(a, ml, size);}
        shift_l1(ml, size);
        shift_r1(mr, size);
    }
}

void mod_bit(uint32_t *a,uint32_t*m, uint8_t size)
{
    uint32_t div[size];
    set_zero(div, size);
    memcpy(div+size/2,m,size/2*sizeof(uint32_t));
    for(int i = 0; i <= size/2 * 32;i++)
    {
        if(!less(a,div, size)) {
            op_minus(a, div, size);
        }
        shift_r1(div, size);
    }
}

void pow_mod_bit(uint32_t *a,uint32_t *b,uint32_t *m, uint8_t size)
{
    uint32_t mr[size];
    uint32_t aux[size];
    cpy(mr, b, size);
    cpy(aux, a, size);
    mod_bit(aux, m, size);
    set_zero(a, size);
    a[0]=1;
    while(!zero(mr, size))
    {  
        if(mr[0] & 1) 
        {
            mul_bit(a, aux, size);
            mod_bit(a, m, size);
        }
        mul_bit(aux,aux, size);
        mod_bit(aux, m, size);
        shift_r1(mr, size);
    }
}

void test_local_key()
{
    uint8_t size = 6;
    uint32_t a[size];
    uint32_t b[size];
    uint32_t g[size];
    uint32_t n[size];
    set_zero(g, size);
    set_zero(n, size);
    g[0] = 2;
    n[2] = 0x10000;
    n[0] = 0xd;

    uint32_t g2[size];
    uint32_t n2[size];
    set_zero(g2, size);
    set_zero(n2, size);
    g2[0] = 2;
    n2[2] = 0x10000;
    n2[0] = 0xd;

    set_rand(a, size);
    set_rand(b, size);

    // print(a,"a");
    pow_mod_bit(g, a, n, size);
    print(g,"A", size);

    // print(b,"b");
    pow_mod_bit(g2, b, n2, size);
    // print(g2,"B");

    pow_mod_bit(g2, a, n, size);
    print(g2,"Ka", size);
    pow_mod_bit(g, b, n2, size);
    // print(g,"Kb");

    xil_printf("done!");
}
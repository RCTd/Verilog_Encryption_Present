#include "diffie-hellman.h"


void set_zero(uint32_t *a, uint8_t size)
{
    for(int i = 0; i < size; i++) {a[i] = 0;}
}
void set_rand(uint32_t *a,uint8_t size)
{
    set_zero(a,size);
    for(int i = 0; i < size/2; i++) {a[i] = (uint32_t) rand();}
}

uint32_t is_less(uint32_t *a, uint32_t *b, uint8_t size)
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

uint32_t is_zero(uint32_t *a, uint8_t size)
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

void send_as_hex(uint32_t *a, char *str, uint8_t size)
{
    if(str)
        xil_printf("%s\r\n",str);
    for (int i = size - 1; i >= 0; i--) {
        xil_printf("%08x",a[i]);
    }
    if(str)
        xil_printf("\r\n");
}

void cpy(uint32_t *a, uint32_t *b, uint8_t size)
{
    memcpy(a ,b, size * sizeof(uint32_t));
}

void shift_left(uint32_t *a, uint8_t size)
{
    for(int i = size-1; i > 0; i--)
    {
        a[i] = a[i] << 1 | a[i-1]>>(32-1);
    }
    a[0] = a[0] << 1;
}

void shift_right(uint32_t *a, uint8_t size)
{
    for(int i = 0; i < size-1; i++)
    {
        a[i] = a[i] >> 1 | a[i+1]<<(32-1);
    }
    a[size-1] = a[size-1] >> 1;
}

void subtraction(uint32_t *a, uint32_t *b, uint8_t size)
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

void addition(uint32_t *a, uint32_t *b, uint8_t size)
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

void multiply(uint32_t *a,uint32_t*b, uint8_t size)
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

void modulo(uint32_t *a,uint32_t*m, uint8_t size)
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

void pow_in_modulo(uint32_t *a,uint32_t *b,uint32_t *m, uint8_t size)
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

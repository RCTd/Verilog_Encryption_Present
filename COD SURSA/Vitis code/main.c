#include "xparameters.h"
#include "xuartlite.h"
#include "diffie-hellman.h"
#include <stdint.h>
#include <string.h>
#include <xil_types.h>
#include "xgpio.h"

//Variabile globale
XUartLite uart0;
XGpio gpio_led_sw;
XGpio gpio_7_seg;

//pozitia segmentului curent afisat 
int seg_pos=0;

uint32_t* encoder=(uint32_t*)(XPAR_PRESENT_ENCODER_0_BASEADDR);
uint32_t* decoder=(uint32_t*)(XPAR_PRESENT_DECODER_0_BASEADDR);

#define word_64 2
#define word_80 3

void recive_64b(uint32_t *b, uint8_t size)
{
    u8 buff;
    uint8_t i=0;
    set_zero(b, size);
    while(i<size*4)
    {
        if(XUartLite_Recv(&uart0,&buff,1))
        {
            // b[(i/4)] = b[(i/4)] | ((uint32_t)buff<<(8*(i%4)));
            b[size-1-(i/4)] = b[size-1-(i/4)] | ((uint32_t)buff<<(8*(3-i%4)));
            i++;
        }
    }
}

void set_key(uint32_t *k)
{
    encoder[3]=k[0];
    encoder[4]=k[1];
    encoder[5]=k[2];
    decoder[3]=k[0];
    decoder[4]=k[1];
    decoder[5]=k[2];

    encoder[0]=1<<2; encoder[0]=0;
    decoder[0]=1<<2; decoder[0]=0;
}

void encode(uint32_t *t)
{
    encoder[4]=t[0];
    encoder[3]=t[1];
    encoder[0]=1<<1; encoder[0]=0;
    while(!(encoder[0]&0x10));
    memcpy( t,(encoder+6),2 * sizeof(*t));
    t[1] = encoder[7];
    t[0] = encoder[6];
}

void decode(uint32_t *t)
{
    decoder[4]=t[1];
    decoder[3]=t[0];
    decoder[0]=1<<1; decoder[0]=0;
    while(!(decoder[0]&0x10));
    t[1] = decoder[7];
    t[0] = decoder[6];
}

void key_agreement()
{
    uint8_t size = word_80 * 2;
    uint32_t a[size];
    set_rand(a, size);
    uint32_t g[size];
    uint32_t p[size];
    set_zero(g, size);
    set_zero(p, size);
    g[0] = 2;
    p[2] = 0x10000;
    p[0] = 0xd;

    pow_in_modulo(g, a, p, size);
    send_as_hex(g,"A", size/2);

    recive_64b(g, size/2);
    pow_in_modulo(g, a, p, size);

    set_key(g);
}

void echo()
{
    uint32_t g[2];
    g[0]=0;
    while(g[0] != 4)
    {
        recive_64b(g, 2);
        send_as_hex(g, 0, 2);
    }
}

void enc_echo()
{
    uint32_t g[3];
    set_zero(g, 3);
    while(1)
    {
        recive_64b(g, 2);
        if(g[0] == 4) {
            send_as_hex(g, 0, 1);
            return;
        } 
        encode(g);
        send_as_hex(g, 0, 2);
    }
}

void dec_echo()
{
    uint32_t g[3];
    set_zero(g, 3);
    while(1)
    {
        recive_64b(g, 2);
        if(g[0] == 4) {
            send_as_hex(g, 0, 1);
            return;
        } 
        decode(g);
        send_as_hex(g, 0, 2);
    }
}


void write_seg(int pos,int val)
{
    switch (val) {
    case 0: val = 0xc0; break;
    case 1: val = 0xf9; break;
    case 2: val = 0xa4; break;
    case 3: val = 0xb0; break;
    case 4: val = 0x99; break;
    case 5: val = 0x92; break;
    case 6: val = 0x82; break;
    case 7: val = 0xf8; break;
    case 8: val = 0x80; break;
    case 9: val = 0x90; break;
    case 10: val = 0x88; break;
    case 11: val = 0x83; break;
    case 12: val = 0xc6; break;
    case 13: val = 0xa1; break;
    case 14: val = 0x86; break;
    case 15: val = 0x8e; break;
    default: val = 0x7f;
    }
    XGpio_DiscreteWrite(&gpio_7_seg,1,0xff);
    XGpio_DiscreteWrite(&gpio_7_seg,2,(~(1<<pos)));
    XGpio_DiscreteWrite(&gpio_7_seg,1,val);
}

// void get_seg_key(uint32_t *key)
void get_key_on_segments()
{
    uint8_t* btns=(uint8_t*)(XPAR_XGPIO_2_BASEADDR);    
    seg_pos=seg_pos+1;
    if (seg_pos > 7) {seg_pos = 0;}
    if(btns[0]>>2<3){
        uint32_t key[3];
        if(!(XGpio_DiscreteRead(&gpio_led_sw,2)&0x1)){
            key[2] = encoder[0]>>16;
            // memcpy(&key[1], encoder + KOUT0_REG, 2 * sizeof(*key));
            key[1] = encoder[2];
            key[0] = encoder[1];
        }else{
            key[2] = decoder[0]>>16;
            // memcpy(&key[1], decoder + KOUT0_REG, 2 * sizeof(*key));
            key[1] = decoder[2];
            key[0] = decoder[1];
        }
        write_seg(seg_pos,(key[btns[0]>>2]>>(4*seg_pos)) & 0xf);
    }else
        write_seg(seg_pos,0);
    
}

int main()
{
    XUartLite_Config uart0_cfg = {     
   		 .RegBaseAddr = XPAR_AXI_UARTLITE_0_BASEADDR,
   		 .BaudRate = XPAR_AXI_UARTLITE_0_BAUDRATE,
   		 .UseParity = XPAR_AXI_UARTLITE_0_USE_PARITY,
   		 .ParityOdd = XPAR_AXI_UARTLITE_0_ODD_PARITY,
         .DataBits = XPAR_AXI_UARTLITE_0_DATA_BITS};
    XUartLite_CfgInitialize(&uart0,
   		 &uart0_cfg, XPAR_AXI_UARTLITE_0_BASEADDR);

    XGpio_Config gpio_led_sw_config = {
	.BaseAddress  	= XPAR_XGPIO_0_BASEADDR,
	.InterruptPresent = XPAR_AXI_GPIO_0_INTERRUPT_PRESENT,
	.IsDual       	= XPAR_AXI_GPIO_0_IS_DUAL };
    XGpio_CfgInitialize(&gpio_led_sw, &gpio_led_sw_config, XPAR_AXI_GPIO_0_BASEADDR);

    XGpio_Config gpio_7_seg_config = {
	.BaseAddress  	= XPAR_AXI_GPIO_1_BASEADDR,
	.InterruptPresent = XPAR_AXI_GPIO_1_INTERRUPT_PRESENT,
	.IsDual       	= XPAR_AXI_GPIO_1_IS_DUAL };
    XGpio_CfgInitialize(&gpio_7_seg, &gpio_7_seg_config, XPAR_AXI_GPIO_1_BASEADDR);

    u8 command;

    while(1)
    {
        while(!XUartLite_Recv(&uart0,&command,1))get_key_on_segments();
        
        switch (command) {
        case 'K': key_agreement(); break;
        case 'N': echo(); break;
        case 'E': enc_echo(); break;
        case 'D': dec_echo(); break;
        default : get_key_on_segments(); break;
        }
        // test_key();
    }

    return 0;   
}
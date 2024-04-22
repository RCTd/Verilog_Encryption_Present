#include "xparameters.h"
#include "xuartlite.h"
#include "diffie-hellman.h"
#include <stdint.h>
#include <xil_types.h>
XUartLite uart0;


void set_key(uint32_t *k)
{
    uint32_t* encoder=(uint32_t*)(XPAR_PRESENT_ENCODER_BLOCK_0_BASEADDR);
    uint32_t* decoder=(uint32_t*)(XPAR_PRESENT_DECODER_BLOCK_0_BASEADDR);
    memcpy((encoder+2), k,3 * sizeof(uint32_t));
    memcpy((decoder+2), k,3 * sizeof(uint32_t));
    encoder[0]=1<<2; encoder[0]=0;
    decoder[0]=1<<2; decoder[0]=0;
}

void encode(uint32_t *t)
{
    uint32_t* encoder=(uint32_t*)(XPAR_PRESENT_ENCODER_BLOCK_0_BASEADDR);
    memcpy((encoder+2), t,3 * sizeof(uint32_t));
    encoder[0]=1<<1; encoder[0]=0;
    while(!encoder[1]);
    memcpy( t,(encoder+5),2 * sizeof(uint32_t));
}

void decode(uint32_t *t)
{
    uint32_t* decoder=(uint32_t*)(XPAR_PRESENT_DECODER_BLOCK_0_BASEADDR);
    memcpy((decoder+2), t,3 * sizeof(uint32_t));
    decoder[0]=1<<1; decoder[0]=0;
    while(!decoder[1]);
    memcpy( t,(decoder+5),2 * sizeof(uint32_t));
}

void recive_B(uint32_t *b, uint8_t size)
{
    u8 buff;
    uint8_t i=0;
    set_zero(b, size);
    while(i<size*4)
    {
        if(XUartLite_Recv(&uart0,&buff,1))
        {
            b[(i/4)] = b[(i/4)] | ((uint32_t)buff<<(8*(i%4)));
            i++;
        }
    }
}

void key_agreement()
{
    uint8_t size = 6;
    uint32_t a[size];
    set_rand(a, size);
    uint32_t g[size];
    uint32_t n[size];
    set_zero(g, size);
    set_zero(n, size);
    g[0] = 2;
    n[2] = 0x10000;
    n[0] = 0xd;

    pow_mod_bit(g, a, n, size);
    print(g,"A", size/2);

    recive_B(g, 3);
    // print(g,"G",3);
    pow_mod_bit(g, a, n, size);

    print(g,"K", size/2);
    set_key(g);
}

void echo()
{
    uint32_t g[2];
    g[0]=0;
    while(g[0] != 4)
    {
        recive_B(g, 2);
        print(g, 0, 2);
    }
}

void enc_echo()
{
    uint32_t g[3];
    set_zero(g, 3);
    while(1)
    {
        recive_B(g, 2);
        if(g[0] == 4) {
            print(g, 0, 1);
            return;
        } 
        encode(g);
        print(g, 0, 2);
    }
}

void dec_echo()
{
    uint32_t g[3];
    set_zero(g, 3);
    while(1)
    {
        recive_B(g, 2);
        if(g[0] == 4) {
            print(g, 0, 1);
            return;
        } 
        decode(g);
        print(g, 0, 2);
    }
}

int main()
{
    XUartLite_Config uart0_cfg = {     
   		//  .DeviceId = XPAR_AXI_UARTLITE_0_DEVICE_ID,
   		 .RegBaseAddr = XPAR_AXI_UARTLITE_0_BASEADDR,
   		 .BaudRate = XPAR_AXI_UARTLITE_0_BAUDRATE,
   		 .UseParity = XPAR_AXI_UARTLITE_0_USE_PARITY,
   		 .ParityOdd = XPAR_AXI_UARTLITE_0_ODD_PARITY,
         .DataBits = XPAR_AXI_UARTLITE_0_DATA_BITS};
    XUartLite_CfgInitialize(&uart0,
   		 &uart0_cfg, XPAR_AXI_UARTLITE_0_BASEADDR);
    XUartLite_Send(&uart0, "Hello world!\r\n", 14);

    char command;
    while(1)
    {
        while(!XUartLite_Recv(&uart0,&command,1));
        
        switch (command) {
        case 'K': key_agreement(); break;
        case 'N': echo(); break;
        case 'E': enc_echo(); break;
        case 'D': dec_echo(); break;
        }
        // test_key();
    }

    return 0;   
}
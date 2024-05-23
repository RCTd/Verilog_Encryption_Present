#include "uart.h"
#include "diffie-hellman.h"
#include "Present.h"
#include "string"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;


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

void recive(HANDLE port,char c, uint32_t* k)
{
    char buff[99];
    int i = 0, j = 0, b = 0;
    uint32_t result[2];
    set_zero(result, 2);
    uint32_t blank[2];
    uint32_t aux;
    do
    {
        i = read_port(port, (uint8_t*)buff, 1);
        if (buff[0] != 0 && i > 0)
        {
            for(int i=0;i<4;i++)
                shift_l1(result,2);
            result[0] = result[0] | hexCharToInt(buff[0]);
            j++;
            b++;
            if (j >= 2) {
                j = 0;
                //if(c == 'N') printf("%0c", result[0]);
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
                    printch(result, 2);
                    break;
                case 'E':
                    encript(result, k);
                    print(result, 2);
                    break;
                case 'D':
                    decript(result, k);
                    aux = result[0];
                    result[0] = result[1];
                    result[1] = aux;
                    printch(result, 2);
                    //printch(result, 2);
                    break;
                default:
                    break;
                }
                set_zero(result, 2);
            }

        }
    } while (1);
}

void SendArray(const char* str, HANDLE port, char c) {
    size_t len = strlen(str);
    size_t i;
    size_t num_values = len;
    uint8_t array[8];
    array[0] = c;
    while (write_port(port, array, 1));
    for (i = 0; i < num_values || i % 8; i++) {
        if(i<num_values)
            array[i%8] = ((unsigned char)str[i]);
        else
            array[i % 8] = 0;

        if (i%8 == 7) {
            this_thread::sleep_for(std::chrono::milliseconds(12));
            write_port(port, array, 8);
        }
    }
    for (int i = 0; i < 7; i++) { array[i] = 0; }
    array[7] = 4;
    write_port(port, array, 8);
}


int main()
{
    string str;
    cout << "Input connection port: ";
    //cin >> str;
    str = "15";
    str = "\\\\.\\COM" + str;
    uint32_t baud_rate = 9600;
    HANDLE port = open_serial_port(str.c_str(), baud_rate);
    if (port == INVALID_HANDLE_VALUE) { return 1; }

    uint32_t k[6];
    uint32_t d[2];
    set_zero(d, 2);
    set_zero(k, 3);
    /*decript(d, k);
    print(d, 2);*/

 /*   k[0] = 0x334;
    k[1] = 0x81e62b18;
    k[2] = 0;*/
    set_key(port, k);
    printf("donekey\n");
    int cnt = 0;
    while (cnt<1) {
        //const char* input_string = "1aaaaaaaaaaaaaaaaaaHelloWorld123 its a me Mario! but the princess is in another castle. Can this text get any longer without problems?\r\n";
        const char* input_string = "1234567890abcdefghijklmnopqrstuvwxyz\r\n";
        //const char* input_string = "abcd";
        //str = "";
        //cin>> str;
        thread thread_1(recive, port, 'N',k);
        SendArray(input_string, port, 'N');
        //SendArray(str.c_str(), port, 'E');
        thread_1.join();
        cnt++;
    }
    //printf("\nfrom fpga\n");

    const char* input_string = "1aaaaaaaaaaaaaaaaaaHelloWorld123 its a me Mario! but the princess is in another castle. Can this text get any longer without problems?\r\n";
    //const char* input_string = "abcd";
    thread thread_1(recive, port, 'D',k);
    SendArray(input_string, port, 'E');
    thread_1.join();


    //set_key(port, k);
    printf("\nkey: ");
    print(k, 3);
    
    CloseHandle(port);
    return 0;
}

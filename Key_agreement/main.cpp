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

void recive(HANDLE port)
{
    char buff[99];
    int i = 0, j = 0;
    uint32_t result = 0;
    do
    {
        i = read_port(port, (uint8_t*)buff, 1);
        if (buff[0] != 0 && i > 0)
        {
            result = (result << 4) | hexCharToInt(buff[0]);
            j++;
            if (j >= 2) {
                j = 0;
                printf("%0c", result);
                if (result == 4) {
                    return;
                }
                result = 0;
            }
        }
    } while (1);
}

void SendArray(const char* str, HANDLE port,char c) {
    size_t len = strlen(str);
    size_t i;
    uint32_t value;
    size_t num_values = (len + 3) / 4;
    uint32_t array[2];
    array[0] = c;
    write_port(port, (uint8_t*)array, 1);
    for (i = 0; i < num_values; i++) {
        value = 0;
        for (size_t j = 0; j < 4; j++) {
            value = (value << 8) | ((unsigned char)str[i * 4 + j]);
        }
        array[1-(i % 2)] = value;
        if (i % 2) {
            write_port(port, (uint8_t*)array, 8);
            this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    if (i % 2)
    {
        array[1 - (i % 2)] = 0;
        write_port(port, (uint8_t*)array, 8);
    }
    array[1] = 0;
    array[0] = 4;
    write_port(port, (uint8_t*)array, 8);
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
    set_key(port, k);
    printf("donekey\n");
    int cnt = 0;
    while (cnt<1) {
        const char* input_string = "HelloWorld123 its a me Mario! but the princess is in another castle. Can this text get any longer without problems?\r\n";
        //str = "";
        //cin>> str;
        thread thread_1(recive, port);
        SendArray(input_string, port,'N');
        thread_1.join();
        cnt++;
    }

    cnt = 0;
    while (cnt < 5) {
        const char* input_string = "HelloWorld123 its a me Mario! but the princess is in another castle. Can this text get any longer without problems?\r\n";
        //str = "";
        //cin >> str;
        thread thread_1(recive, port);
        //SendArray(str.c_str(), port, 'E');
        SendArray(input_string, port, 'D');
        thread_1.join();
        cnt++;
    }

    set_key(port, k);

    CloseHandle(port);
    return 0;
}

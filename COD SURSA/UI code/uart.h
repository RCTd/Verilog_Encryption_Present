#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <windows.h>

SSIZE_T read_port(HANDLE port, uint8_t* buffer, size_t size);
int write_port(HANDLE port, uint8_t* buffer, size_t size);
HANDLE open_serial_port(const char* device, uint32_t baud_rate);

#endif // !UART_H

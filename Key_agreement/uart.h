#ifndef UART_H
#define UART_H

#include <stdio.h>
#include <stdint.h>
#include <windows.h>

int init(HANDLE port);
int jrk_get_feedback(HANDLE port);
int jrk_get_target(HANDLE port);
int jrk_get_variable(HANDLE port, uint8_t offset, uint8_t* buffer,
    uint8_t length);
int jrk_set_target(HANDLE port, uint16_t target);
SSIZE_T read_port(HANDLE port, uint8_t* buffer, size_t size);
int write_port(HANDLE port, uint8_t* buffer, size_t size);
HANDLE open_serial_port(const char* device, uint32_t baud_rate);
void print_error(const char* context);

#endif // !UART_H

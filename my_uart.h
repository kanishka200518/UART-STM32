#ifndef MY_UART_H
#define MY_UART_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void my_uart_init(void);
void _putchar(char c);
void my_uart_init_assert(void);
void my_uart_trace_assert(const char *string);

// Add this line to expose our safe processing loop
void my_uart_process_tx(void); 

#ifdef __cplusplus
}
#endif

#endif

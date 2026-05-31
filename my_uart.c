#include "my_uart.h"
#include "ring_buffer.h"
#include <stm32c0xx_hal.h> 
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
    size_t Serial_write_wrapper(uint8_t c);
#ifdef __cplusplus
}
#endif

static ring_buffer_t tx_buffer;
static bool initialized = false;

void my_uart_init(void) {
    if (initialized) return;
    ring_buffer_init(&tx_buffer);
    initialized = true;
}

// Synchronized Processing: Emempties the queue byte-by-byte without 
// stepping on incoming background reception memory lines.
void my_uart_process_tx(void) {
    uint8_t c = 0;
    if (ring_buffer_get(&tx_buffer, &c)) {
        Serial_write_wrapper(c); 
    }
}

void _putchar(char c) {
    if (c == '\n') {
        _putchar('\r');
    }

    // If the buffer is full, bleed one byte out before adding more
    while (ring_buffer_full(&tx_buffer)) {
        my_uart_process_tx();
    }

    // Deposit raw value into tracking queue
    ring_buffer_put(&tx_buffer, (uint8_t)c);
}

void my_uart_init_assert(void) {
}

static void uart_putchar_polling(char c) {
    if (c == '\n') {
        uart_putchar_polling('\r');
    }
    Serial_write_wrapper((uint8_t)c);
}

void my_uart_trace_assert(const char *string) {
    int i = 0;
    while (string[i] != '\0') {
        uart_putchar_polling(string[i]);
        i++;
    }
}

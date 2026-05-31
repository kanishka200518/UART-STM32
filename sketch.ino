#include "my_uart.h"

extern "C" size_t Serial_write_wrapper(uint8_t c) {
    return Serial.write(c);
}

void setup() {
    Serial.begin(115200);
    delay(500); 

    my_uart_init();
    
    my_uart_trace_assert("=== CORE SYSTEM TERMINAL ONLINE ===\n");
    my_uart_trace_assert("You can  type characters or paste paragraphs below:\n\n");
}

void loop() {
    // 1. Fetch exactly one incoming character from the physical console
    if (Serial.available() > 0) {
        char incoming_byte = (char)Serial.read();
        _putchar(incoming_byte);
    }
    
    // 2. Deliver exactly one outgoing character to the display screen
    my_uart_process_tx();
    
    // 3. Give the web simulation environment a tiny bit of breathing room
    delayMicroseconds(50);
}

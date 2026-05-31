#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

// Massive storage buffer zone to absorb deep paragraphs completely
#define BUFFER_SIZE 512  

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    volatile uint16_t head;  
    volatile uint16_t tail;  
} ring_buffer_t;

static inline void ring_buffer_init(ring_buffer_t *rb) {
    rb->head = 0;
    rb->tail = 0;
}

static inline bool ring_buffer_full(ring_buffer_t *rb) {
    return ((rb->head + 1) % BUFFER_SIZE) == rb->tail;
}

static inline bool ring_buffer_empty(ring_buffer_t *rb) {
    return rb->head == rb->tail;
}

static inline bool ring_buffer_put(ring_buffer_t *rb, uint8_t byte) {
    // Prevent overwrite collision
    if (ring_buffer_full(rb)) {
        return false; 
    }
    rb->buffer[rb->head] = byte;
    rb->head = (rb->head + 1) % BUFFER_SIZE;
    return true;
}

static inline bool ring_buffer_get(ring_buffer_t *rb, uint8_t *byte) {
    if (ring_buffer_empty(rb)) {
        return false;
    }
    *byte = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % BUFFER_SIZE;
    return true;
}

#endif

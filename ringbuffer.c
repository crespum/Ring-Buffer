#include "ringbuffer.h"
#include <string.h>

bool ring_buffer_init(ring_buffer_t *rb, ring_buffer_attr_t attr) { 
    if ((rb != NULL)) {
        if ((attr.buffer != NULL) && (attr.size_elem > 0)) {
            /* Check that the size of the ring buffer is a power of 2 */
            if (((attr.num_elem - 1) & attr.num_elem) == 0) {
                /* Initialize the ring buffer internal variables */
                rb->head = 0;
                rb->tail = 0;
                rb->buffer = attr.buffer;
                rb->size_elem = attr.size_elem;
                rb->num_elem = attr.num_elem;
 
                return true;
            }
        }
    }
 
    return false;
}

bool ring_buffer_put(ring_buffer_t *rb, const void *data) {
    if (!ring_buffer_is_full(rb)) {
        const uint16_t offset = (rb->head & (rb->num_elem - 1)) * rb->size_elem;
        memcpy(&(((uint8_t *) rb->buffer)[offset]), data, rb->size_elem);
        rb->head++;
        return true;
    } else {
        return false;
    }
 }

bool ring_buffer_get(ring_buffer_t *rb, void *data) { 
    if (!ring_buffer_is_empty(rb)) {
        const uint16_t offset = (rb->tail & (rb->num_elem - 1)) * rb->size_elem;
        memcpy(data, &(((uint8_t *) rb->buffer)[offset]), rb->size_elem);
        rb->tail++;
        return true;
    } else {
        return false;
    }
}

bool ring_buffer_peek(ring_buffer_t *rb, void *data, ring_buffer_size_t index) {
    if (!ring_buffer_is_empty(rb) && ring_buffer_num_items(rb)>index) {
        const uint16_t offset = (rb->tail + index & (rb->num_elem - 1)) * rb->size_elem;
        memcpy(data, &(((uint8_t *) rb->buffer)[offset]), rb->size_elem);
        return true;
    } else {
        return false;
    }
}

extern inline bool ring_buffer_is_empty(ring_buffer_t *rb);
extern inline bool ring_buffer_is_full(ring_buffer_t *rb);
extern inline uint8_t ring_buffer_num_items(ring_buffer_t *rb);

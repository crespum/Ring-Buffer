#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include <stdint.h>
#include <stdbool.h>

/**
 * Type used to hold the size and the indexes of the buffer.
 */
typedef uint16_t ring_buffer_size_t;

/**
 * Structure to configure the buffer format (number of elements, size of each
 * element and pointer to the buffer).
 */
typedef struct {
    ring_buffer_size_t size_elem;
    ring_buffer_size_t num_elem;
    void *buffer;
} ring_buffer_attr_t;

/**
 * Structure which holds a ring buffer (data buffer + metadata).
 */
typedef struct {
    void *buffer;
    ring_buffer_size_t tail;
    ring_buffer_size_t head;
    ring_buffer_size_t size_elem;
    ring_buffer_size_t num_elem;
} ring_buffer_t;

/**
 * Initializes the ring buffer pointed to by <em>buffer</em>.
 * This function can also be used to empty/reset the buffer.
 *
 * @param rb The ring buffer to initialize.
 * @param attr The ring buffer attributes.
 *
 * @return true if success; false otherwise.
 */
bool ring_buffer_init(ring_buffer_t *rb, ring_buffer_attr_t attr);

/**
 * Adds a byte to a ring buffer.
 *
 * @param rb The buffer in which the data should be placed.
 * @param data The byte to place.
 *
 * @return true if data was queued; false otherwise.
 */
bool ring_buffer_put(ring_buffer_t *rb, const void *data);

/**
 * Returns the oldest byte in a ring buffer.
 *
 * @param rb The buffer from which the data should be returned.
 * @param data A pointer to the location at which the data should be placed.
 *
 * @return true if data was returned; false otherwise.
 */
bool ring_buffer_get(ring_buffer_t *rb, void *data);

/**
 * Peeks a ring buffer, i.e. returns an element without removing it.
 *
 * @param rb The buffer from which the data should be returned.
 * @param data A pointer to the location at which the data should be placed.
 * @param index The index to peek.
 *
 * @return true if data was returned; false otherwise.
 */
bool ring_buffer_peek(ring_buffer_t *rb, void *data, ring_buffer_size_t index);

/**
 * Removes the first element in the queue from the buffer.
 *
 * @param rb The buffer from which the data should be returned.
 *
 * @return true the item was dropped; false otherwise.
 */
bool ring_buffer_drop(ring_buffer_t *rb);

/**
 * Returns whether a ring buffer is empty.
 *
 * @param rb The buffer for which it should be returned whether it is empty.
 *
 * @return true if empty; false otherwise.
 */
inline bool ring_buffer_is_empty(ring_buffer_t *rb) {
    return (rb->head - rb->tail) == 0U;
}

/**
 * Returns whether a ring buffer is full.
 *
 * @param rb The buffer for which it should be returned whether it is full.
 *
 * @return true if full; false otherwise.
 */
inline bool ring_buffer_is_full(ring_buffer_t *rb) {
    return (rb->head - rb->tail) == rb->num_elem;
}

/**
 * Returns the number of items in a ring buffer.
 *
 * @param rb The buffer for which the number of items should be returned.
 *
 * @return The number of items in the ring buffer.
 */
inline ring_buffer_size_t ring_buffer_num_items(ring_buffer_t *rb) {
    return rb->head - rb->tail;
}

#endif /* __RING_BUFFER_H__ */

Ring-Buffer
===========
[![CircleCI](https://circleci.com/gh/crespum/Ring-Buffer.svg?style=svg)](https://circleci.com/gh/crespum/Ring-Buffer)

A simple ring buffer (circular buffer) designed for embedded systems. It allows creating queues for different types of elements.

Code based on the following two projects:
 * http://www.simplyembedded.org/tutorials/interrupt-free-ring-buffer/
 * https://github.com/crespum/Ring-Buffer

# Usage
```c
#define RING_BUFFER_MAX_NUM_ELEMENTS     16

ring_buffer_t ring_buffer;

uint8_t buffer[RING_BUFFER_MAX_NUM_ELEMENTS];
ring_buffer_attr_t attr = {
    .buffer = buffer,
    .size_elem = sizeof(buffer[0]),
    .num_elem = sizeof(buffer),
};

ring_buffer_init(&ring_buffer, attr);

uint8_t buf_put = 8;
ring_buffer_put(&ring_buffer, &buf_put);

uint8_t buff_get;
ring_buffer_get(&ring_buffer, &buf_get);
```
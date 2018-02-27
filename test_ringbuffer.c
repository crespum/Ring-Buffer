#include "unity.h"

#include "ringbuffer.h"


#define RING_BUFFER_MAX_NUM_ELEMENTS     16

ring_buffer_t ring_buffer;

uint8_t buffer[RING_BUFFER_MAX_NUM_ELEMENTS];
ring_buffer_attr_t attr = {
    .buffer = buffer,
    .size_elem = sizeof(buffer[0]),
    .num_elem = sizeof(buffer),
};

uint8_t buf_put = 25;
uint8_t buf_get = 0;


void setUp(void) {
    TEST_ASSERT_TRUE(ring_buffer_init(&ring_buffer, attr));
}

void tearDown(void) {
}

void test_should_ReturnBufferEmpty_when_NoDataQueued(void) {
    TEST_ASSERT_EQUAL(0, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_TRUE(ring_buffer_is_empty(&ring_buffer));
    TEST_ASSERT_FALSE(ring_buffer_get(&ring_buffer, &buf_get));
}

void test_should_ReturnBufferEmpty_when_DataAddedAndRemoved(void) {
    TEST_ASSERT_TRUE(ring_buffer_put(&ring_buffer, &buf_put));
    TEST_ASSERT_TRUE(ring_buffer_get(&ring_buffer, &buf_get));

    TEST_ASSERT_EQUAL(0, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_TRUE(ring_buffer_is_empty(&ring_buffer));
}

void test_should_ReturnBufferNotEmpty_when_SingleElementQueued(void) {
    TEST_ASSERT_TRUE(ring_buffer_put(&ring_buffer, &buf_put));
    TEST_ASSERT_EQUAL(1, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_FALSE(ring_buffer_is_empty(&ring_buffer));
}

void test_should_ReturnBufferFull_when_MaxSizeReached(void) {
    for(buf_put=0 ; buf_put<RING_BUFFER_MAX_NUM_ELEMENTS ; buf_put++)
        TEST_ASSERT_TRUE(ring_buffer_put(&ring_buffer, &buf_put));

    TEST_ASSERT_EQUAL(RING_BUFFER_MAX_NUM_ELEMENTS, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_TRUE(ring_buffer_is_full(&ring_buffer));
}

void test_should_ReturnBufferNotFull_when_OneElementLeft(void) {
    for(buf_put=0 ; buf_put<RING_BUFFER_MAX_NUM_ELEMENTS-1 ; buf_put++)
        TEST_ASSERT_TRUE(ring_buffer_put(&ring_buffer, &buf_put));

    TEST_ASSERT_EQUAL(RING_BUFFER_MAX_NUM_ELEMENTS-1, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_FALSE(ring_buffer_is_full(&ring_buffer));
}

void test_should_KeepSize_when_BufferPeek(void) {
    TEST_ASSERT_TRUE(ring_buffer_put(&ring_buffer, &buf_put));

    TEST_ASSERT_TRUE(ring_buffer_peek(&ring_buffer, &buf_get, 0));
    TEST_ASSERT_EQUAL(1, ring_buffer_num_items(&ring_buffer));
}

void test_should_ReturnError_when_PeekNonExistingElement(void) {
    TEST_ASSERT_TRUE(ring_buffer_put(&ring_buffer, &buf_put));

    TEST_ASSERT_FALSE(ring_buffer_peek(&ring_buffer, &buf_get, 1));
}

void test_should_ReturnCorrectElement_when_BufferPeek(void) {
    // Fill buffer
    for(buf_put=0 ; buf_put<5 ; buf_put++)
        ring_buffer_put(&ring_buffer, &buf_put);

    TEST_ASSERT_TRUE(ring_buffer_peek(&ring_buffer, &buf_get, 0));
    TEST_ASSERT_EQUAL(0, buf_get);

    TEST_ASSERT_TRUE(ring_buffer_peek(&ring_buffer, &buf_get, 4));
    TEST_ASSERT_EQUAL(4, buf_get);

    // Remove one item
    TEST_ASSERT_TRUE(ring_buffer_get(&ring_buffer, &buf_get));
    TEST_ASSERT_EQUAL(0, buf_get);

    TEST_ASSERT_TRUE(ring_buffer_peek(&ring_buffer, &buf_get, 0));
    TEST_ASSERT_EQUAL(1, buf_get); // Element 0 corresponds to tail

    TEST_ASSERT_TRUE(ring_buffer_peek(&ring_buffer, &buf_get, 3));
    TEST_ASSERT_EQUAL(4, buf_get); // Element 4 corresponds to head
}

void test_should_ReturnFirstElement_when_Unqueueing(void) {
    // Add elements to buffer; one at a time
    for(buf_put=0 ; buf_put<5 ; buf_put++)
        TEST_ASSERT_TRUE(ring_buffer_put(&ring_buffer, &buf_put));

    TEST_ASSERT_EQUAL(5, ring_buffer_num_items(&ring_buffer));

    uint8_t i_get;
    for(i_get=0 ; i_get<5 ; i_get++) {
        TEST_ASSERT_TRUE(ring_buffer_get(&ring_buffer, &buf_get));
        TEST_ASSERT_EQUAL(buf_get, i_get);
    }
}

void test_should_DiscardNewElement_when_BufferFull(void) {
    for(buf_put=0 ; buf_put<RING_BUFFER_MAX_NUM_ELEMENTS ; buf_put++)
        TEST_ASSERT_TRUE(ring_buffer_put(&ring_buffer, &buf_put));
    TEST_ASSERT_FALSE(ring_buffer_put(&ring_buffer, &buf_put));

    TEST_ASSERT_TRUE(ring_buffer_get(&ring_buffer, &buf_get));

    TEST_ASSERT_EQUAL(0, buf_get); // First element would be 1 if the element had been added
}

void test_should_ReturnRightElement_when_FillBufferEmptyHalfRefill(void) {
    // Add elements to buffer; one at a time
    for(buf_put=0 ; buf_put<RING_BUFFER_MAX_NUM_ELEMENTS ; buf_put++)
        TEST_ASSERT_TRUE(ring_buffer_put(&ring_buffer, &buf_put));

    TEST_ASSERT_EQUAL(RING_BUFFER_MAX_NUM_ELEMENTS, ring_buffer_num_items(&ring_buffer));

    // Read half of the buffer
    uint8_t i_get;
    for(i_get=0 ; i_get<RING_BUFFER_MAX_NUM_ELEMENTS/2 ; i_get++) {
        TEST_ASSERT_TRUE(ring_buffer_get(&ring_buffer, &buf_get));
        TEST_ASSERT_EQUAL(buf_get, i_get);
    }

    TEST_ASSERT_EQUAL(RING_BUFFER_MAX_NUM_ELEMENTS/2, ring_buffer_num_items(&ring_buffer));

    // Refill the buffer
    for(buf_put=0 ; buf_put<RING_BUFFER_MAX_NUM_ELEMENTS/2 ; buf_put++)
        TEST_ASSERT_TRUE(ring_buffer_put(&ring_buffer, &buf_put));

    TEST_ASSERT_EQUAL(RING_BUFFER_MAX_NUM_ELEMENTS, ring_buffer_num_items(&ring_buffer));

    // Read first half of the buffer
    i_get = RING_BUFFER_MAX_NUM_ELEMENTS/2;
    uint8_t i;
    for(i=0 ; i<RING_BUFFER_MAX_NUM_ELEMENTS/2 ; i++) {
        TEST_ASSERT_TRUE(ring_buffer_get(&ring_buffer, &buf_get));
        TEST_ASSERT_EQUAL(buf_get, i_get++);
    }

    TEST_ASSERT_EQUAL(RING_BUFFER_MAX_NUM_ELEMENTS/2, ring_buffer_num_items(&ring_buffer));

    // Read second half of the buffer
    for(i_get=0 ; i_get<RING_BUFFER_MAX_NUM_ELEMENTS/2 ; i_get++) {
        TEST_ASSERT_TRUE(ring_buffer_get(&ring_buffer, &buf_get));
        TEST_ASSERT_EQUAL(buf_get, i_get);
    }

    TEST_ASSERT_EQUAL(0, ring_buffer_num_items(&ring_buffer));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_should_ReturnBufferEmpty_when_NoDataQueued);
    RUN_TEST(test_should_ReturnBufferEmpty_when_DataAddedAndRemoved);

    RUN_TEST(test_should_ReturnBufferNotEmpty_when_SingleElementQueued);

    RUN_TEST(test_should_ReturnBufferFull_when_MaxSizeReached);

    RUN_TEST(test_should_ReturnBufferNotFull_when_OneElementLeft);

    RUN_TEST(test_should_KeepSize_when_BufferPeek);
    RUN_TEST(test_should_ReturnError_when_PeekNonExistingElement);
    RUN_TEST(test_should_ReturnCorrectElement_when_BufferPeek);

    RUN_TEST(test_should_ReturnFirstElement_when_Unqueueing);
    RUN_TEST(test_should_DiscardNewElement_when_BufferFull);
    RUN_TEST(test_should_ReturnRightElement_when_FillBufferEmptyHalfRefill);

    return UNITY_END();
}

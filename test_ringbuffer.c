#include "unity.h"

#include "ringbuffer.h"

ring_buffer_t ring_buffer;

void setUp(void) {
    // Initialize ring buffer before each test
    ring_buffer_init(&ring_buffer);
}

void tearDown(void) {
}

void test_should_ReturnBufferEmpty_when_NoDataQueued(void) {
    char buf;

    TEST_ASSERT_EQUAL(0, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_TRUE(ring_buffer_is_empty(&ring_buffer));
    TEST_ASSERT_FALSE(ring_buffer_dequeue(&ring_buffer, &buf));
}

void test_should_ReturnBufferEmpty_when_DataAddedAndRemoved(void) {
    char buf;

    ring_buffer_queue(&ring_buffer, 20);
    ring_buffer_dequeue(&ring_buffer, &buf);

    TEST_ASSERT_EQUAL(0, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_TRUE(ring_buffer_is_empty(&ring_buffer));
}

void test_should_ReturnBufferEmpty_when_UnqueingArrayEqualThanNumElementsBuffer(void) {
    int i;
    char buf_arr[5];

    for(i = 0; i < 5; i++)
        ring_buffer_queue(&ring_buffer, i);

    char cnt = ring_buffer_dequeue_arr(&ring_buffer, buf_arr, 5);
    TEST_ASSERT_EQUAL(0, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_TRUE(ring_buffer_is_empty(&ring_buffer));
}

void test_should_ReturnBufferEmpty_when_UnqueingArrayBiggerThanNumElementsBuffer(void) {
    int i;
    char buf_arr[5];

    for(i = 0; i < 5; i++)
        ring_buffer_queue(&ring_buffer, i);

    char cnt = ring_buffer_dequeue_arr(&ring_buffer, buf_arr, 6);
    TEST_ASSERT_EQUAL(0, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_TRUE(ring_buffer_is_empty(&ring_buffer));
}

void test_should_ReturnBufferNotEmpty_when_SingleElementQueued(void) {

    ring_buffer_queue(&ring_buffer, 20);
    TEST_ASSERT_EQUAL(1, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_FALSE(ring_buffer_is_empty(&ring_buffer));
}

void test_should_ReturnBufferFull_when_MaxSizeReached(void) {
    int i;

    for(i = 0; i < RING_BUFFER_MAX_NUM_ELEMENTS; i++)
        ring_buffer_queue(&ring_buffer, i);

    TEST_ASSERT_EQUAL(RING_BUFFER_MAX_NUM_ELEMENTS, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_TRUE(ring_buffer_is_full(&ring_buffer));
}

void test_should_ReturnBufferFull_when_OneExtraElementAdded(void) {
    int i;

    for(i = 0; i < RING_BUFFER_MAX_NUM_ELEMENTS+1; i++)
        ring_buffer_queue(&ring_buffer, i);

    TEST_ASSERT_EQUAL(RING_BUFFER_MAX_NUM_ELEMENTS, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_TRUE(ring_buffer_is_full(&ring_buffer));
}

void test_should_ReturnBufferFull_when_QueingArrayEqualThanBuffer(void) {
    char array[RING_BUFFER_MAX_NUM_ELEMENTS];
    ring_buffer_queue_arr(&ring_buffer, array, sizeof(array));

    TEST_ASSERT_EQUAL(RING_BUFFER_MAX_NUM_ELEMENTS, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_TRUE(ring_buffer_is_full(&ring_buffer));
}


void test_should_ReturnBufferFull_when_QueingArrayBiggerThanBuffer(void) {
    char array[RING_BUFFER_MAX_NUM_ELEMENTS+1];
    ring_buffer_queue_arr(&ring_buffer, array, sizeof(array));

    TEST_ASSERT_EQUAL(RING_BUFFER_MAX_NUM_ELEMENTS, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_TRUE(ring_buffer_is_full(&ring_buffer));
}

void test_should_ReturnBufferNotFull_when_OneElementLeft(void) {
    int i;

    for(i = 0; i < RING_BUFFER_MAX_NUM_ELEMENTS-1; i++)
        ring_buffer_queue(&ring_buffer, i);

    TEST_ASSERT_EQUAL(RING_BUFFER_MAX_NUM_ELEMENTS-1, ring_buffer_num_items(&ring_buffer));
    TEST_ASSERT_FALSE(ring_buffer_is_full(&ring_buffer));
}

void test_should_KeepSize_when_BufferPeek(void) {
    char buf;

    ring_buffer_queue(&ring_buffer, 20);

    ring_buffer_peek(&ring_buffer, &buf, 1);
    TEST_ASSERT_EQUAL(1, ring_buffer_num_items(&ring_buffer));
}

void test_should_ReturnError_when_PeekNonExistingElement(void) {
    char buf;
    bool result;

    ring_buffer_queue(&ring_buffer, 20);

    result = ring_buffer_peek(&ring_buffer, &buf, 2);
    TEST_ASSERT_FALSE(result)
}

void test_should_ReturnCorrectElement_when_BufferPeek(void) {
    char i, buf;

    // Add elements to buffer; one at a time
    for(i = 0; i < 5; i++)
        ring_buffer_queue(&ring_buffer, i);

    ring_buffer_peek(&ring_buffer, &buf, 0);
    TEST_ASSERT_EQUAL(0, buf);

    ring_buffer_peek(&ring_buffer, &buf, 4);
    TEST_ASSERT_EQUAL(4, buf);
}

void test_should_ReturnFirstElement_when_Unqueueing(void) {
    char i, cnt;
    char buf;

    // Add elements to buffer; one at a time
    for(i = 0; i < 5; i++)
        ring_buffer_queue(&ring_buffer, i);

    TEST_ASSERT_EQUAL(5, ring_buffer_num_items(&ring_buffer));

    for(cnt = 0; cnt < 5; cnt++) {
        ring_buffer_dequeue(&ring_buffer, &buf);
        TEST_ASSERT_EQUAL(buf, cnt);
    }
}

void test_should_ReturnFirstElement_when_UnqueueingArray(void) {
    char array[5] = "Fobar";
    char buf_arr[5];
    char cnt;

    ring_buffer_queue_arr(&ring_buffer, array, sizeof(array));

    TEST_ASSERT_EQUAL(5, ring_buffer_num_items(&ring_buffer));

    // Get first 3 items
    cnt = ring_buffer_dequeue_arr(&ring_buffer, buf_arr, 3);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(array, buf_arr, 3);
    TEST_ASSERT_EQUAL(3, cnt);

    TEST_ASSERT_EQUAL(2, ring_buffer_num_items(&ring_buffer));

    // Get remaining 2 items
    cnt = ring_buffer_dequeue_arr(&ring_buffer, buf_arr, 2);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(array+3, buf_arr, 2);
    TEST_ASSERT_EQUAL(2, cnt);

    TEST_ASSERT_EQUAL(0, ring_buffer_num_items(&ring_buffer));
}


void test_should_ReturnAllElements_when_UnqueingArrayBiggerThanNumElementsBuffer(void) {
    char array[5] = "Fobar";
    char buf_arr[5];

    ring_buffer_queue_arr(&ring_buffer, array, sizeof(array));

    TEST_ASSERT_EQUAL(5, ring_buffer_num_items(&ring_buffer));

    char cnt = ring_buffer_dequeue_arr(&ring_buffer, buf_arr, sizeof(array)+1);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(array, buf_arr, sizeof(array));
    TEST_ASSERT_EQUAL(5, cnt);
}

void test_should_DiscardFirstElement_when_BufferOverflows(void) {
    int i;
    char buf;

    for(i = 0; i < RING_BUFFER_MAX_NUM_ELEMENTS+1; i++)
        ring_buffer_queue(&ring_buffer, i);

    ring_buffer_dequeue(&ring_buffer, &buf);
    TEST_ASSERT_EQUAL(1, buf); // 0 has been discarded
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_should_ReturnBufferEmpty_when_NoDataQueued);
    RUN_TEST(test_should_ReturnBufferEmpty_when_DataAddedAndRemoved);
    RUN_TEST(test_should_ReturnBufferEmpty_when_UnqueingArrayEqualThanNumElementsBuffer);
    RUN_TEST(test_should_ReturnBufferEmpty_when_UnqueingArrayBiggerThanNumElementsBuffer);

    RUN_TEST(test_should_ReturnBufferNotEmpty_when_SingleElementQueued);

    RUN_TEST(test_should_ReturnBufferFull_when_MaxSizeReached);
    RUN_TEST(test_should_ReturnBufferFull_when_OneExtraElementAdded);
    RUN_TEST(test_should_ReturnBufferFull_when_QueingArrayEqualThanBuffer);
    RUN_TEST(test_should_ReturnBufferFull_when_QueingArrayBiggerThanBuffer);

    RUN_TEST(test_should_ReturnBufferNotFull_when_OneElementLeft);

    RUN_TEST(test_should_KeepSize_when_BufferPeek);
    RUN_TEST(test_should_ReturnError_when_PeekNonExistingElement);
    RUN_TEST(test_should_ReturnCorrectElement_when_BufferPeek);

    RUN_TEST(test_should_ReturnFirstElement_when_Unqueueing);
    RUN_TEST(test_should_ReturnFirstElement_when_UnqueueingArray);
    RUN_TEST(test_should_ReturnAllElements_when_UnqueingArrayBiggerThanNumElementsBuffer);
    RUN_TEST(test_should_DiscardFirstElement_when_BufferOverflows);

    return UNITY_END();
}

enable_testing()

add_library(test STATIC test.c)

add_executable(test_buffer_empty test_buffer_empty.c)
target_link_libraries(test_buffer_empty test BERT m)
add_test(buffer_empty test_buffer_empty)

add_executable(test_buffer_length test_buffer_length.c)
target_link_libraries(test_buffer_length test BERT m)
add_test(buffer_length test_buffer_length)

add_executable(test_buffer_small_write test_buffer_small_write.c)
target_link_libraries(test_buffer_small_write test BERT m)
add_test(buffer_write_small test_buffer_small_write)

add_executable(test_buffer_big_write test_buffer_big_write.c)
target_link_libraries(test_buffer_big_write test BERT m)
add_test(buffer_write_big test_buffer_big_write)

add_executable(test_buffer_small_read test_buffer_small_read.c)
target_link_libraries(test_buffer_small_read test BERT m)
add_test(buffer_read_small test_buffer_small_read)

add_executable(test_buffer_big_read test_buffer_big_read.c)
target_link_libraries(test_buffer_big_read test BERT m)
add_test(buffer_read_big test_buffer_big_read)

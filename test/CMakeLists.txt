enable_testing()

add_library(test STATIC test.c)

add_executable(test_buffer_init test_buffer_init.c)
target_link_libraries(test_buffer_init test BERT m)
add_test(buffer_init test_buffer_init)

add_executable(test_buffer_empty test_buffer_empty.c)
target_link_libraries(test_buffer_empty test BERT m)
add_test(buffer_empty test_buffer_empty)

add_executable(test_buffer_length test_buffer_length.c)
target_link_libraries(test_buffer_length test BERT m)
add_test(buffer_length test_buffer_length)

add_executable(test_buffer_write_small test_buffer_write_small.c)
target_link_libraries(test_buffer_write_small test BERT m)
add_test(buffer_write_small test_buffer_write_small)

add_executable(test_buffer_write_big test_buffer_write_big.c)
target_link_libraries(test_buffer_write_big test BERT m)
add_test(buffer_write_big test_buffer_write_big)

add_executable(test_buffer_read_small test_buffer_read_small.c)
target_link_libraries(test_buffer_read_small test BERT m)
add_test(buffer_read_small test_buffer_read_small)

add_executable(test_buffer_read_big test_buffer_read_big.c)
target_link_libraries(test_buffer_read_big test BERT m)
add_test(buffer_read_big test_buffer_read_big)

add_executable(test_buffer_clear test_buffer_clear.c)
target_link_libraries(test_buffer_clear test BERT m)
add_test(buffer_clear test_buffer_clear)

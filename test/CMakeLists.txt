add_library(test STATIC test.c)

add_executable(test_buffer_empty test_buffer_empty.c)
target_link_libraries(test_buffer_empty test BERT m)

add_executable(test_buffer_small_write test_buffer_small_write.c)
target_link_libraries(test_buffer_small_write test BERT m)

add_executable(test_buffer_read test_buffer_read.c)
target_link_libraries(test_buffer_read test BERT m)

enable_testing()

add_library(test STATIC test.c)

add_executable(test_decode_small_int test_decode_small_int.c)
target_link_libraries(test_decode_small_int test BERT)
add_test(decode_small_int test_decode_small_int)

add_executable(test_decode_big_int test_decode_big_int.c)
target_link_libraries(test_decode_big_int test BERT)
add_test(decode_big_int test_decode_big_int)

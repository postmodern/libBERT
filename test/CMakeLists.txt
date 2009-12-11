enable_testing()

add_library(test STATIC test.c)

add_executable(test_decode_small_int test_decode_small_int.c)
target_link_libraries(test_decode_small_int test BERT)
add_test(decode_small_int test_decode_small_int)

add_executable(test_decode_big_int test_decode_big_int.c)
target_link_libraries(test_decode_big_int test BERT)
add_test(decode_big_int test_decode_big_int)

add_executable(test_decode_float test_decode_float.c)
target_link_libraries(test_decode_float test BERT)
add_test(decode_float test_decode_float)

add_executable(test_decode_atom test_decode_atom.c)
target_link_libraries(test_decode_atom test BERT)
add_test(decode_atom test_decode_atom)

add_executable(test_decode_string test_decode_string.c)
target_link_libraries(test_decode_string test BERT)
add_test(decode_string test_decode_string)

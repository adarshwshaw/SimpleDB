add_compile_options(-I${CMAKE_SOURCE_DIR})
add_executable(blocktest blocktest.c ${CMAKE_SOURCE_DIR}/block.c)

add_test(NAME blocktest
    COMMAND blocktest 0)

add_executable(pagetest pagetest.c ${CMAKE_SOURCE_DIR}/page.c)

add_test(NAME pagetest
    COMMAND pagetest 0)
add_test(NAME pagetest1
    COMMAND pagetest 1)
add_test(NAME pagetest2
    COMMAND pagetest 2)

add_executable(utiltest utiltest.c)
add_test(NAME utiltest
    COMMAND utiltest 0)
add_test(NAME utiltest1
    COMMAND utiltest 1)

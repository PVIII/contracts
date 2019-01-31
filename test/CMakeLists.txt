cmake_minimum_required(VERSION 3.8)

add_executable(contracts_test
               contracts_audit_test.cpp
               contracts_example.cpp
               contracts_test)
target_link_libraries(contracts_test PUBLIC contracts)
target_include_directories(contracts_test PUBLIC
                           ${CMAKE_CURRENT_BINARY_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(contracts_test INTERFACE cxx_std_14)

add_test(NAME test COMMAND contracts_test)

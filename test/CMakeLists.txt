cmake_minimum_required(VERSION 3.8)

add_executable(contracts_test
               contracts_audit_test.cpp
               contracts_example.cpp
               contracts_test.cpp)
target_link_libraries(contracts_test PUBLIC contracts CONAN_PKG::Catch2)

add_test(NAME test COMMAND contracts_test)

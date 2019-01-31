cmake_minimum_required(VERSION 3.8)
project(contracts)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
else()
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
endif()
conan_basic_setup()

enable_testing()

add_library(contracts INTERFACE)
target_sources(contracts INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/include/contracts/contracts.hpp)
target_include_directories(contracts INTERFACE include/)
target_compile_features(contracts INTERFACE cxx_std_17)

add_subdirectory(test)

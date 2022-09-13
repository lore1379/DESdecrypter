cmake_minimum_required(VERSION 3.3)
project(DESdecrypter)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -lcrypt")

add_executable(DESdecrypter main.cpp DictionaryAttack.cpp DictionaryAttack.h)
target_link_libraries(DESdecrypter crypt)
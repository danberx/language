cmake_minimum_required(VERSION 3.21)
project(Language)

set(CMAKE_CXX_STANDARD 20)

add_executable(language main.cpp lexemes.h words.txt bor.h)

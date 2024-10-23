cmake_minimum_required(VERSION 3.21)
project(Language)

set(CMAKE_CXX_STANDARD 20)

add_executable(language main.cpp words.txt bor.cpp bor.hpp lexeme.hpp lexeme.cpp lexemetype.hpp lexicalanalyzer.cpp lexicalanalyzer.hpp)

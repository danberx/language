cmake_minimum_required(VERSION 3.21)
project(Language)

set(CMAKE_CXX_STANDARD 20)
include_directories(src/bor src/txt src/lexemeanalyzer)
add_executable(language src/main.cpp src/txt/operations.txt src/txt/words.txt src/bor/bor.cpp src/bor/bor.hpp src/lexemeanalyzer/lexeme.hpp src/lexemeanalyzer/lexeme.cpp src/lexemeanalyzer/lexemetype.hpp src/lexemeanalyzer/lexicalanalyzer.cpp src/lexemeanalyzer/lexicalanalyzer.hpp code.txt)
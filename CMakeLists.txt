cmake_minimum_required(VERSION 3.21)
project(Language)

set(CMAKE_CXX_STANDARD 20)
include_directories(src/bor src/txt src/lexemeanalyzer src/semanticanalyzer)
add_executable(test src/main.cpp src/txt/operations.txt src/txt/words.txt src/bor/bor.cpp src/bor/bor.hpp src/lexemeanalyzer/lexeme.hpp src/lexemeanalyzer/lexeme.cpp src/lexemeanalyzer/lexemetype.hpp src/lexemeanalyzer/lexicalanalyzer.cpp src/lexemeanalyzer/lexicalanalyzer.hpp code.txt src/syntacticanalyzer/syntacticanalyzer.cpp src/semanticanalyzer/semanticanalyzer.cpp src/semanticanalyzer/semanticanalyzer.hpp src/bor/borsem.cpp src/bor/borsem.hpp src/semanticanalyzer/type.hpp
        src/bor/borfunc.cpp src/bor/borfunc.hpp
        src/generation/poliz.cpp
)
cmake_minimum_required(VERSION 3.12)
project(untitled1)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled1 main.cpp Lex_analyzer.h Grammer_analyzer.h Grammer_analyzer.cpp Lex_analyzer.cpp Operatorpriority_analyzer.h Operatorpriority_analyzer.cpp middle_codes_production.h)
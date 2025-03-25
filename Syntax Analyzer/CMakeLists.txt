cmake_minimum_required(VERSION 3.10)
project(PascalParser)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加源文件
add_executable(parser
    main.cpp
    recursive_parser.cpp
    predictive_parser.cpp
    lexer.cpp
)

# 添加头文件目录
target_include_directories(parser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) 
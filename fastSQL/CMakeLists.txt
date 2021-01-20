cmake_minimum_required(VERSION 3.17)
project(fastSQL)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(fastSQL
        main.cpp
        main.h
        config/config.cpp
        conn_handler/connection_acceptor.cpp
        sql/lex.cpp
        sql/tokens.h
        utils/string_utils.cpp)


target_link_libraries(fastSQL
        PRIVATE pthread
        PRIVATE m)
cmake_minimum_required(VERSION 3.17)
project(fastSQL)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(fastSQL
        main.cpp
        main.h
        config/config.cpp
        conn_handler/connection_acceptor.cpp
        table/row.cpp
        sql/lex.cpp
        sql/parser.cpp
        sql/prepared.cpp
        sql/dml/select.cpp
        sql/dml/query.cpp
        index/cursor.cpp
        utils/file_utils.cpp
        utils/string_utils.cpp
        utils/date.cpp
        value/value.cpp)


target_link_libraries(fastSQL
        PRIVATE pthread
        PRIVATE m)
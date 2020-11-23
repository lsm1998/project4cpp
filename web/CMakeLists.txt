project(web)

set(src
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.h
        ${CMAKE_CURRENT_SOURCE_DIR}/task.h
        ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool.h)
add_executable(web ${src})
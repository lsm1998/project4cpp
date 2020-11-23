project(tests)

add_executable(sever
        ${CMAKE_CURRENT_SOURCE_DIR}/sever.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sever.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/socket.h)

add_executable(client
        ${CMAKE_CURRENT_SOURCE_DIR}/client.h
        ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/socket.h)
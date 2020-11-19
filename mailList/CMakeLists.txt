project(mailList)

set(src
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.h
        ${CMAKE_CURRENT_SOURCE_DIR}/node.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mailList.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mailList.h
        ${CMAKE_CURRENT_SOURCE_DIR}/list.h)
add_executable(mailList ${src})
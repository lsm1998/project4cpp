project(redis)

set(SRC_SERVER server.cpp aof.cpp structure/list.h t_string.cpp t_hash.cpp t_list.cpp redis.h)

set(SRC_CLI client.cpp)

add_executable(redis_server ${SRC_SERVER})
add_executable(redis_client ${SRC_CLI})
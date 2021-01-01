project(courage C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-g -lm -Wall -I object -I vm -I compiler -I parser -I include -I cli -I gc -W -Wstrict-prototypes -Wmissing-prototypes -Wsystem-headers -fgnu89-inline")

add_executable(courage
        main.c main.h
        include/common.h include/unicodeUtf8.c include/unicodeUtf8.h include/utils.h include/utils.c
        parser/parser.c parser/parser.h
        gc/gc.c gc/gc.h
        compiler/compiler.c compiler/compiler.h compiler/debug.c compiler/debug.h
        vm/core.h vm/core.c vm/vm.c vm/vm.h
        object/obj_fn.c object/obj_fn.h
        object/class.c object/class.h object/header_obj.c object/header_obj.h object/meta_obj.c object/meta_obj.h
        object/obj_map.c object/obj_map.h object/obj_list.c object/obj_list.h object/obj_range.c object/obj_range.h
        object/obj_string.c object/obj_string.h object/obj_thread.c object/obj_thread.h)
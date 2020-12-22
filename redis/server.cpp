//
// Created by 刘时明 on 2020/12/22.
//
#include "structure/list.h"

int main()
{
    auto *list = new List<int*>();
    list->add_node(0, new int(1));
    list->add_node(1, new int(2));
    list->add_node(2, new int(3));
    list->add_node(3, new int(4));
    list->add_node(4, new int(5));
    //list->add_node(0,100);

    list->foreach([](int* value) -> void
                  {
                      std::cout << *value << std::endl;
                  });

    list->delete_node(2);
    list->delete_node(1);
    list->delete_node(0);
    list->foreach([](int* value) -> void
                  {
                      std::cout << *value << std::endl;
                  });
    return 0;
}
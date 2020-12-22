//
// Created by 刘时明 on 2020/12/22.
//

#ifndef PROJECT4CPP_LIST_H
#define PROJECT4CPP_LIST_H

#include <type_traits>
#include <exception>
#include <iostream>

template<typename T>
class ListNode
{
public:
    ListNode() = default;

    ListNode(T value) : value(value)
    {};

    ~ListNode()
    {
        // 如果是指针类型，则delete
        if (std::is_pointer<T>::value)
        {
            delete value;
        }
    }

private:
    // 前置节点
    ListNode<T> *prev;

    // 后置节点
    ListNode<T> *next;

    // 节点的值
    T value;

    template<typename E>
    friend
    class List;
};

template<typename T>
class List
{
private:
    // 表头节点
    ListNode<T> *head;

    // 表尾节点
    ListNode<T> *tail;

    // 链表所包含的节点数量
    unsigned long len;

public:
    List();

    ~List();

    void add_node_head(T value);

    void add_node_tail(T value);

    void add_node(T value);

    void add_node(int index, T value);

    void foreach(void(*func)(T value)) const;

    T get(int index) const;

    [[nodiscard]] int size() const
    {
        return this->len;
    }

    void delete_node(int index);

private:
    ListNode<T> *get_node(int index) const;

    void insert_node(T value, ListNode<T> *node, bool is_before);

    void delete_node(ListNode<T> *node);
};

template<typename T>
List<T>::List()
{
    // 初始化属性
    this->head = this->tail = nullptr;
    this->len = 0;
}

template<typename T>
List<T>::~List()
{
    ListNode<T> *current, *next;
    // 指向头指针
    current = this->head;
    // 遍历整个链表
    long current_len = this->len;
    while (current_len--)
    {
        next = current->next;
        delete current;
        current = next;
    }
}

template<typename T>
void List<T>::add_node_head(T value)
{
    auto *node = new ListNode<T>(value);

    if (this->len == 0)
    {
        this->head = this->tail = node;
        node->next = node->prev = nullptr;
    } else
    {
        node->prev = nullptr;
        node->next = this->head;
        this->head->prev = node;
        this->head = node;
    }
    this->len++;
}

template<typename T>
void List<T>::add_node_tail(T value)
{
    auto *node = new ListNode<T>(value);

    // 目标链表为空
    if (this->len == 0)
    {
        this->head = this->tail = node;
        node->prev = node->next = nullptr;
        // 目标链表非空
    } else
    {
        node->prev = this->tail;
        node->next = nullptr;
        this->tail->next = node;
        this->tail = node;
    }
    // 更新链表节点数
    this->len++;
}

template<typename T>
void List<T>::insert_node(T value, ListNode<T> *node, bool is_before)
{
    auto *new_node = new ListNode<T>(value);
    // 是否插入在之前
    if (is_before)
    {
        new_node->next = node;
        new_node->prev = node->prev;
        // 给定节点是原表头节点
        if (this->head == node)
        {
            this->head = new_node;
        }
    } else
    {
        new_node->prev = node;
        new_node->next = node->next;
        // 给定节点是原表尾节点
        if (this->tail == node)
        {
            this->tail = new_node;
        }
    }
    // 更新新节点的前置指针
    if (new_node->prev != nullptr)
    {
        new_node->prev->next = new_node;
    }
    // 更新新节点的后置指针
    if (new_node->next != nullptr)
    {
        new_node->next->prev = new_node;
    }
    // 更新链表节点数
    this->len++;
}

template<typename T>
void List<T>::delete_node(ListNode<T> *node)
{
    // 调整前置节点的指针
    if (node->prev)
    {
        node->prev->next = node->next;
    } else
    {
        this->head = node->next;
    }
    // 调整后置节点的指针
    if (node->next)
    {
        node->next->prev = node->prev;
    } else
    {
        this->tail = node->prev;
    }
    delete node;
    this->len--;
}

template<typename T>
void List<T>::add_node(int index, T value)
{
    if (index < 0 || index > this->len)
    {
        throw "index out of range";
    }
    // 是否空表
    if (this->len == 0)
    {
        this->add_node_head(value);
        return;
    }
    this->insert_node(value, this->get_node(index - 1), false);
}

template<typename T>
ListNode<T> *List<T>::get_node(int index) const
{
    if (index > this->len / 2)
    {
        index = -(this->len - index);
    }
    ListNode<T> *node;
    // 如果索引为负数，从表尾开始查找
    if (index < 0)
    {
        index = (-index) - 1;
        node = this->tail;
        while (index-- && node) node = node->prev;
        // 如果索引为正数，从表头开始查找
    } else
    {
        node = this->head;
        while (index-- && node) node = node->next;
    }
    return node;
}

template<typename T>
void List<T>::foreach(void (*func)(T)) const
{
    // 是否空表
    if (this->len == 0)
    {
        return;
    }
    ListNode<T> *node = this->head;
    while (node)
    {
        func(node->value);
        node = node->next;
    }
}

template<typename T>
T List<T>::get(int index) const
{
    if (index < 0 || index > this->len - 1)
    {
        throw "index out of range";
    }
    return this->get_node(index)->value;
}

template<typename T>
void List<T>::add_node(T value)
{
    this->add_node(this->len, value);
}

template<typename T>
void List<T>::delete_node(int index)
{
    if (index < 0 || index > this->len - 1)
    {
        throw "index out of range";
    }
    this->delete_node(this->get_node(index));
}

#endif //PROJECT4CPP_LIST_H

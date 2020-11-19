/**
* 作者：刘时明
* 时间：2020/11/15 0015
*/
#ifndef PROJECT4CPP_LIST_H
#define PROJECT4CPP_LIST_H

#include "node.h"

template<typename T>
class LinkedList
{
private:
    Node<T> *head;
    Node<T> *tail;
    int _size{0};

public:
    LinkedList();

    ~LinkedList();

    int size();

    void add(T data);

    void add(int index, T data);

    T get(int index);

    int indexOf(T data);

    // void remove(T data);

    void remove(int index);

    bool checkIndex(int index);

    bool checkIndexAdd(int index);
};

template<typename T>
LinkedList<T>::~LinkedList()
{
    Node<T> temp = this->head;
    while (temp != nullptr)
    {
        delete temp;
        temp = temp.next;
    }
}

template<typename T>
int LinkedList<T>::size()
{
    return this->_size;
}

template<typename T>
void LinkedList<T>::add(T data)
{
    this->add(this->size(), data);
}

template<typename T>
void LinkedList<T>::add(int index, T data)
{
    if (!this->checkIndexAdd(index))
    {
        printf("访问越界!!! \n");
        return;
    }
    this->_size++;
    Node<T> *temp = this->head;
    if (temp == nullptr)
    {
        this->head = new Node((Node<T> *) nullptr, (Node<T> *) nullptr, data);
        this->tail = new Node<T>;
        this->head->next = this->tail;
        this->tail->prev = this->head;
        return;
    }
    int count = 0;
    while (++count < index)
    {
        temp = temp->next;
    }
    Node<T> *newNode = new Node(temp, temp->next, data);
    temp->next = newNode;
}

template<typename T>
bool LinkedList<T>::checkIndex(int index)
{
    return index < this->size() && index >= 0;
}

template<typename T>
bool LinkedList<T>::checkIndexAdd(int index)
{
    return index <= this->size() && index >= 0;
}

template<typename T>
T LinkedList<T>::get(int index)
{
    if (!this->checkIndex(index))
    {
        printf("访问越界!!! \n");
        // 基本数值类型返回nullptr会报错
        // return nullptr;
        return NULL;
    }
    Node<T> *temp = this->head;
    int count = 0;
    while (count++ < index)
    {
        temp = temp->next;
    }
    return temp->data;
}

template<typename T>
int LinkedList<T>::indexOf(T data)
{
    Node<T> *temp = this->head;
    int index = 0;
    while (temp != NULL)
    {
        if (temp->data == data)
        {
            return index;
        }
        temp = temp->next;
        index++;
    }
    return -1;
}

//template<typename T>
//void LinkedList<T>::remove(T data)
//{
//    int index = this->indexOf(data);
//    if (index >= 0)
//    {
//        this->remove(index);
//    }
//}

template<typename T>
void LinkedList<T>::remove(int index)
{
    if (!checkIndex(index))
    {
        printf("访问越界!!! \n");
        return;
    }
    Node<T> *temp = this->head;

    // 删除的是否头节点
    if (index == 0)
    {
        this->head = temp->next;
        delete temp;
    } else
    {
        Node<T> *prev = nullptr;
        int count = 0;
        while (count++ < index)
        {
            prev = temp;
            temp = temp->next;
        }
        prev->next = temp->next;
        delete temp;
    }
    this->_size--;
}

template<typename T>
LinkedList<T>::LinkedList()
{
    this->head= nullptr;
    this->tail = nullptr;
}

#endif //PROJECT4CPP_LIST_H

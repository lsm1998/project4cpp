/**
* 作者：刘时明
* 时间：2020/11/15 0015
*/
#ifndef PROJECT4CPP_NODE_H
#define PROJECT4CPP_NODE_H

template<typename T>
class Node
{
private:
    T data;
    Node *prev;
    Node *next;

    template<typename E>
    friend
    class LinkedList;

public:
    Node() = default;

    Node(const Node &node);

    Node(Node *prev, Node *next, T data);

    ~Node();
};


template<typename T>
Node<T>::Node(Node *prev, Node *next, T data)
{
    this->prev = prev;
    this->next = next;
    this->data = data;
}

template<typename T>
Node<T>::Node(const Node &node)
{
    this->prev = new Node<T>;
    this->next = new Node<T>;
    this->data = node.data;
}

template<typename T>
Node<T>::~Node()
{
    this->prev = nullptr;
    this->next = nullptr;
    delete this->data;
}


#endif //PROJECT4CPP_NODE_H

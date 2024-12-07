#ifndef SINGLYLINKEDLIST_H_INCLUDED
#define SINGLYLINKEDLIST_H_INCLUDED

#include <iostream>
#include <stdexcept>

template <typename T>
class LinkedList {
protected:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* head;
    size_t size;

    virtual void Print() const = 0;
    void ProtectedCopyFrom(const LinkedList& other);

public:
    LinkedList();
    LinkedList(const LinkedList& other);
    LinkedList(LinkedList&& other) noexcept;
    virtual ~LinkedList();

    LinkedList& operator=(const LinkedList& other);
    LinkedList& operator=(LinkedList&& other) noexcept;

    virtual void Push(const T& value) = 0;
    virtual void Pop() = 0;
    virtual T& GetFront() = 0;
    virtual const T& GetFront() const = 0;
    virtual bool IsEmpty() const = 0;
    virtual size_t Size() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const LinkedList& list) {
        list.Print();
        return os;
    }
    friend std::istream& operator>>(std::istream& is, LinkedList& list) {
        T value;
        while (is >> value) {
            list.Push(value);
        }
        return is;
    }

private:
    void Clear();
    void CopyFrom(const LinkedList& other);
};

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), size(0) {}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : head(nullptr), size(0) {
    CopyFrom(other);
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList&& other)noexcept : head(other.head), size(other.size){
    other.head = nullptr;
    other.size = 0;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
    Clear();
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other)
{
    if (this != &other)
    {
        Clear();
        CopyFrom(other);
    }
    return *this;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) noexcept
{
    if (this != &other)
    {
        Clear();
        head = other.head;
        size = other.size;
        other.head = nullptr;
        other.size = 0;
    }
    return *this;
}

template <typename T>
void LinkedList<T>::CopyFrom(const LinkedList& other)
{
    if (other.head == nullptr) return;
    Node* temp = other.head;
    Node* new_node = new Node(temp->data);
    head = new_node;
    temp = temp->next;

    while (temp)
    {
        new_node->next = new Node(temp->data);
        new_node = new_node->next;
        temp = temp->next;
    }
    size = other.size;
}

template <typename T>
void LinkedList<T>::ProtectedCopyFrom(const LinkedList& other)
{
    CopyFrom(other);
}

template <typename T>
void LinkedList<T>::Clear()
{
    while (head)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    size = 0;
}


// Реализация стека

template <typename T>
class Stack : public LinkedList<T> {
public:
    Stack();
    ~ Stack();
    Stack(const Stack& other);
    Stack(Stack&& other) noexcept;
    Stack& operator=(const Stack& other);
    Stack& operator=(Stack&& other) noexcept;

    void Push(const T& value) override;
    void Pop() override;
    T& GetFront() override;
    const T& GetFront() const override;
    bool IsEmpty() const override;
    size_t Size() const override;
    void Print() const override;

private:
    using LinkedList<T>::head;
    using LinkedList<T>::size;
    void CopyFrom(const Stack& other);
    void Clear();
};

template <typename T>
Stack<T>::Stack() : LinkedList<T>() {}

template <typename T>
Stack<T>::Stack(const Stack& other) : LinkedList<T>(other) {}

template <typename T>
Stack<T>::Stack(Stack&& other) noexcept : LinkedList<T>(std::move(other)) {}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& other)
{
    if (this != &other)
    {
        LinkedList<T>::operator=(other);
    }
    return *this;
}

template <typename T>
Stack<T>& Stack<T>::operator=(Stack&& other) noexcept
{
    if (this != &other)
    {
        LinkedList<T>::operator=(std::move(other));
    }
    return *this;
}

template <typename T>
Stack<T>::~Stack() {}

template <typename T>
void Stack<T>::Push(const T& value)
{
    typename LinkedList<T>::Node* new_node = new typename LinkedList<T>::Node(value);
    new_node->next = this->head;
    this->head = new_node;
    this->size++;
}

template <typename T>
void Stack<T>::Pop()
{
    if (IsEmpty())
    {
        throw std::runtime_error("Stack is empty");
    }
    typename LinkedList<T>::Node* temp = this->head;
    this->head = this->head->next;
    delete temp;
    this->size--;
}

template <typename T>
T& Stack<T>::GetFront()
{
    if (IsEmpty()) throw std::runtime_error("Stack is empty");
    return this->head->data;
}

template <typename T>
const T& Stack<T>::GetFront() const
{
    if (IsEmpty()) throw std::runtime_error("Stack is empty");
    return this->head->data;
}

template <typename T>
bool Stack<T>::IsEmpty() const
{
    return this->head == nullptr;
}

template <typename T>
size_t Stack<T>::Size() const
{
    return this->size;
}

template <typename T>
void Stack<T>::Print() const {
    typename LinkedList<T>::Node* current = this->head;
    while (current) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

// Реализация очереди

template <typename T>
class Queue : public LinkedList<T> {
public:
    Queue();
    ~Queue();
    Queue(const Queue& other);
    Queue(Queue&& other) noexcept;
    Queue& operator=(const Queue& other);
    Queue& operator=(Queue&& other) noexcept;

    void Push(const T& value) override;
    void Pop() override;
    T& GetFront() override;
    const T& GetFront() const override;
    bool IsEmpty() const override;
    size_t Size() const override;
    void Print() const override;

private:

    typename LinkedList<T>::Node* tail;
    using LinkedList<T>::head;
    using LinkedList<T>::size;

    void CopyFrom(const Queue<T>& other);
    void Clear();
};

template <typename T>
Queue<T>::Queue() : LinkedList<T>() {}

template <typename T>
Queue<T>::Queue(const Queue& other) : LinkedList<T>(other), tail(nullptr)
{
    if (other.head == nullptr)
    {
        return;
    }
    typename LinkedList<T>::Node* temp = this->head;
    while (temp->next)
    {
        temp = temp->next;
    }
    tail = temp;
}

template <typename T>
Queue<T>::Queue(Queue&& other) noexcept : LinkedList<T>(std::move(other)), tail(other.tail)
{
    other.tail = nullptr;
}

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue& other)
{
    if (this != &other)
    {
        LinkedList<T>::operator=(other);

        typename LinkedList<T>::Node* temp = this->head;
        while (temp && temp->next)
        {
            temp = temp->next;
        }
        tail = temp;
    }
    return *this;
}

template <typename T>
Queue<T>& Queue<T>::operator=(Queue&& other) noexcept
{
    if (this != &other)
    {
        LinkedList<T>::operator=(std::move(other));
        tail = other.tail;
        other.tail = nullptr;
    }
    return *this;
}

template <typename T>
Queue<T>::~Queue() {}

template <typename T>
void Queue<T>::Push(const T& value)
{
    typename LinkedList<T>::Node* new_node = new typename LinkedList<T>::Node(value);
    if (this->IsEmpty())
    {
        this->head = new_node;
    }
    else
    {
        tail->next = new_node;
    }
    tail = new_node;
    this->size++;
}

template <typename T>
void Queue<T>::Pop()
{
    if (IsEmpty())
    {
        throw std::runtime_error("Queue is empty");
    }
    typename LinkedList<T>::Node* temp = this->head;
    this->head = this->head->next;
    delete temp;

    if (this->head == nullptr)
    {
        tail = nullptr;
    }

    this->size--;
}

template <typename T>
T& Queue<T>::GetFront()
{
    if (IsEmpty()) throw std::runtime_error("Queue is empty");
    return this->head->data;
}
template <typename T>
const T& Queue<T>::GetFront() const
{
    if (IsEmpty()) throw std::runtime_error("Queue is empty");
    return this->head->data;
}

template <typename T>
bool Queue<T>::IsEmpty() const
{
    return this->head == nullptr;
}

template <typename T>
size_t Queue<T>::Size() const
{
    return this->size;
}

template <typename T>
void Queue<T>::Print() const
{
    typename LinkedList<T>::Node* temp = head;
    while (temp)
    {
        std::cout << temp->data << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
}


template <typename T>
void Queue<T>::CopyFrom(const Queue<T>& other)
{
    this->ProtectedCopyFrom(other);
    typename LinkedList<T>::Node* temp = this->head;
    while (temp && temp->next)
    {
        temp = temp->next;
    }
    tail = temp;
}


#endif // SINGLYLINKEDLIST_H_INCLUDED

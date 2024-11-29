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

        Node(T val) : data(val), next(nullptr) {}
    };

    Node* head;
    size_t size;

    virtual void Print() const = 0;

public:
    LinkedList();
    LinkedList(const LinkedList& other);
    virtual ~LinkedList();
    virtual void Push(T value) = 0;
    virtual T Pop() = 0;
    virtual T GetFront() const = 0;
    virtual bool IsEmpty() const;
    virtual size_t Size() const;

    LinkedList& operator=(const LinkedList& other);
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
};

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), size(0) {}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : head(nullptr), size(0) {
    Node* current = other.head;
    Node* lastInserted = nullptr;                                                       // Указатель на последний вставленный элемент
    while (current) {
        Node* newNode = new Node(current->data);                                        // Создание нового узла
        if (!head) {                                                                    // Если список пуст, новый узел становится головным
            head = newNode;
        } else {
            lastInserted->next = newNode;                                               // Привязываем новый узел к списку
        }
        lastInserted = newNode;                                                         // Последний добавленный узел
        current = current->next;                                                        // Переход к следующему узлу
    }
    size = other.size;                                                                  // Копируем размер
}

template <typename T>
LinkedList<T>::~LinkedList() {
    while (!IsEmpty()) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T>
bool LinkedList<T>::IsEmpty() const { return size == 0; }

template <typename T>
size_t LinkedList<T>::Size() const { return size; }

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this != &other) {
        while (!IsEmpty()) {
            Pop();
        }
        Node* current = other.head;
        while (current) {
            Push(current->data);
            current = current->next;
        }
    }
    return *this;
}

// Реализация стека

template <typename T>
class Stack : public LinkedList<T> {
public:
    Stack();
    void Push(T value) override;
    T Pop() override;
    T GetFront() const override;
    void Print() const override;
};

template <typename T>
Stack<T>::Stack() : LinkedList<T>() {}

template <typename T>
void Stack<T>::Push(T value) {
    auto newNode = new typename LinkedList<T>::Node(value);
    newNode->next = this->head;
    this->head = newNode;
    this->size++;
}

template <typename T>
T Stack<T>::Pop() {
    if (this->IsEmpty()) {
        throw std::out_of_range("Stack is empty");
    }
    T value = this->head->data;
    typename LinkedList<T>::Node* temp = this->head;
    this->head = this->head->next;
    delete temp;
    this->size--;
    return value;
}

template <typename T>
T Stack<T>::GetFront() const {
    if (this->IsEmpty()) {
        throw std::out_of_range("Stack is empty");
    }
    return this->head->data;
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
    void Push(T value) override;
    T Pop() override;
    T GetFront() const override;
    void Print() const override;
};

template <typename T>
Queue<T>::Queue() : LinkedList<T>() {}

template <typename T>
void Queue<T>::Push(T value) {
    auto newNode = new typename LinkedList<T>::Node(value);
    if (this->IsEmpty()) {
        this->head = newNode;
    } else {
        typename LinkedList<T>::Node* current = this->head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
    this->size++;
}

template <typename T>
T Queue<T>::Pop() {
    if (this->IsEmpty()) {
        throw std::out_of_range("Queue is empty");
    }
    T value = this->head->data;
    typename LinkedList<T>::Node* temp = this->head;
    this->head = this->head->next;
    delete temp;
    this->size--;
    return value;
}

template <typename T>
T Queue<T>::GetFront() const {
    if (this->IsEmpty()) {
        throw std::out_of_range("Queue is empty");
    }
    return this->head->data;
}

template <typename T>
void Queue<T>::Print() const {
    typename LinkedList<T>::Node* current = this->head;
    while (current) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

#endif // SINGLYLINKEDLIST_H_INCLUDED

#include <iostream>
#include "SinglyLinkedList.h"

int main() {
    Stack<int> stack;
    stack.Push(10);
    stack.Push(20);
    std::cout << "Stack " << stack << std::endl;

    Queue<int> queue;
    queue.Push(30);
    queue.Push(40);
    std::cout << "Queue: " << queue << std::endl;

    return 0;
}

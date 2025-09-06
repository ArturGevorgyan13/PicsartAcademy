#include <iostream>
#include <thread>

struct Node{

    int val;

    Node* next;

};

Node* head = nullptr;

void add() {

    for(int i = 0; i < 100; ++i) {

        Node* newnode = new Node;

        newnode->next = head;

        head = newnode;

    }

}

int main()

{

    std::thread t1(add);
    std::thread t2(add);
    std::thread t3(add);

    t1.join();
    t2.join();
    t3.join();

    int count = 0;

    while(head != nullptr) {

        ++count;

        head = head->next;

    }

    std::cout<<count;

    return 0;

}
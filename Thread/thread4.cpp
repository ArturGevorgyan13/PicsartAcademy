#include <iostream>
#include <thread>
#include <string>

std::string message;

void add(int id) {

    for(int i = 0; i < 100; ++i) {

        message.append("Thread " + std::to_string(id));

    }

}

int main()

{

    std::thread t1(add, 1);
    std::thread t2(add, 2);
    std::thread t3(add, 3);
    
    t1.join();
    t2.join();
    t3.join();

    std::cout<<message.size();

    std::cout<<message;

    return 0;

}
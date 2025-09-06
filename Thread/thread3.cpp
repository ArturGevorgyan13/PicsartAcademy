#include <iostream>
#include <thread>
#include <vector>

std::vector<int> vec;

void add(int id) {

    for(int i = 0; i < 1000; ++i) {

        vec.push_back(id * 1000 + i);

    }

    std::cout<<vec.size();

}

int main()

{

    std::thread t1(add, 1);
    std::thread t2(add, 2);
    std::thread t3(add, 3);
    std::thread t4(add, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;

}
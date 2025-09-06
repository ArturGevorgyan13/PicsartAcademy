#include <iostream>
#include <thread>
#include <fstream>

void write(int id) {

    std::ofstream file("output.txt", std::ios::app);

    for(int i = 1; i <= 100; ++i) {

        file << "Thread " << id << ": Line " << i << "\n";

    }

}

int main()

{

    std::thread t1(write, 1);
    std::thread t2(write, 2);
    std::thread t3(write, 3);
    std::thread t4(write, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;

}
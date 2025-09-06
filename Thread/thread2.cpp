#include <iostream>
#include <thread>

int global = 0;

void inc() {

    for(int i = 0; i < 10000; ++i) ++global;

}

int main()

{

    std::thread t1(inc);
    std::thread t2(inc);
    std::thread t3(inc);
    std::thread t4(inc);
    std::thread t5(inc);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    std::cout<<global;

    return 0;

}
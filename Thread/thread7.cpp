#include <iostream>
#include <thread>
#include <climits>
#include <ctime>

int sum = 0;

int count = 0;

int max_val = INT_MIN;

int min_val = INT_MAX;

void random_func() {

    for(int i = 0; i < 10000; ++i) {

        int num = 1 + std::rand() % 101;

        sum += num;

        ++count;

        if(num > max_val) max_val = num;
        if(num < min_val) min_val = num;

    }

}

int main()

{

    std::srand(std::time(0));

    std::thread t1(random_func);
    std::thread t2(random_func);
    std::thread t3(random_func);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Average: " << sum / count << std::endl;
    std::cout << "Max: " << max_val << std::endl;
    std::cout << "Min: " << min_val << std::endl;

    return 0;

}
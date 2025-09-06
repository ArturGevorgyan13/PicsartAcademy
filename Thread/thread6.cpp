#include <iostream>
#include <thread>

int buffer[10];

int count = 0;

void producer() {

    for(int i = 1; i <= 50; ++i) {

        if(count < 10) {

            buffer[count] = i;

            ++count;

            std::cout << "Produced: " << i <<"\n";

        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }

}

void consumer() {

    for(int i = 1; i <= 50; ++i) {

        if(count > 0) {

            --count;

            int val = buffer[count];

            std::cout << "Consumed: " << val << "\n";

        }

        else {

            std::cout << "Consumed: ??? (empty)\n";

        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    }

}

int main()

{

    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    return 0;

}
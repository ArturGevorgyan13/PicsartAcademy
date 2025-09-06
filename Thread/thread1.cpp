#include <iostream>
#include <thread>

int account_balance = 1000;

void withdraw(int person_id) {

    --account_balance;

    std::cout<<"Person "<<person_id<<": "<<account_balance<<std::endl;

}

int main()

{

    std::thread p1(withdraw, 1);
    std::thread p2(withdraw, 2);
    std::thread p3(withdraw, 3);

    p1.join();
    p2.join();
    p3.join();

    std::cout<<account_balance;

    return 0;

}
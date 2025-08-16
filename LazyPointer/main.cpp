#include "lazypointer.h"

struct Mlass{

    std::string name;

    double money;

    Mlass(std::string n, double m) : name{n}, money{m} {

        std::cout<<"Object is created!\n";

    }

    void info() const {

        std::cout<<"Person "<<name<<" has "<<money<<" money!\n";

    }

};

int main()

{

    lazy_ptr<Mlass> p("Artur",1000);

    std::cout<<p.is_constructed()<<std::endl;

    p->info();

    std::cout<<p.is_constructed()<<std::endl;

    if(p) {

        std::cout<<"Object is exists!\n";

    }

    p.reset();

    std::cout<<p.is_constructed()<<std::endl;

    return 0;

}
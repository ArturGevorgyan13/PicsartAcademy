//In my program I want to introduce how the auto type decution works,
//I created different variables and showed their types deduced by auto.


//In comments for any variable I use word var!

//Here are some rules that help me to understand type deduction with auto,
//at first it seems hard but consider this rules and i think it will be supriseingly easy.

//1)For auto it is easy we must look at the assigned variable type,
//  if it is a int so the type of auto will be int, for double -> double, for complex type will be the same complex type.

//2)For auto& same rule as first but if for example auto was deduced as int so the variable type will be int&, etc

//3)For auto&& we have a deal with universal references...
//Here if we assign rvalue(int for example) then the type of auto will be int, but the type of variable will be int&&.
//Here if we assign lvalue(int for example) then the type of auto will be int&,
//but the type of variable will be int&(because of reference collapsing & + && -> &, & + & -> &, && + & -> &, && + && -> &&).

//Same rules when we use const with auto so types will be const int, const int& etc...

//Some important notes...

//1)When we use const auto&&, this is not an universal refernce it is an rvalue reference

//2)When we use auto variable = {1, 2, ...}, the type is initializes_list

//3)We can't write likes this auto var; because compiler need to know the type of var

#include <iostream>
#include <typeinfo>
#include <cxxabi.h>
#include <memory>
#include <string>
#include <vector>

double GLOBAL = 7.6;

template <typename T>
std::string type_info() {

    int status = 0;

    std::unique_ptr<char, void(*)(void*)> res {

        abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status),

        std::free

    };

    return (status == 0) ? res.get() : typeid(T).name();

}

double& funcR() {

    return GLOBAL;

}

int func() {

    int tmp = 9;

    return tmp;

}

int main()

{

//Here the type of var is int, beacuse 1 is literal and it's type is int     
    auto varInt = 1;
    std::cout<<"The type of varInt is "<<type_info<decltype(varInt)>()<<std::endl;

//Here the type of var is int&&, because 1 is literal so auto is deduced int and var's type is int&&
    auto&& varRRInt = 1;
    static_assert(std::is_same_v<decltype(varRRInt), int&&>, "Not int&&");

    std::cout<<"The type of varRRInt "<<type_info<decltype(varRRInt)>()<<std::endl;

//Here the type of var is double because 1.6 if floating-point literal and it's type is double
    auto varDouble = 1.6;
    std::cout<<"The type of varDouble "<<type_info<decltype(varDouble)>()<<std::endl;
    
//Here the type of var is double, because it is preferable to cast int to double instead of casting double to int    
    auto varDoubleCalc = 6.7 + 15;
    std::cout<<"The type of varDoubleCalc "<<type_info<decltype(varDoubleCalc)>()<<std::endl;

//Here the type of var is int because func()'s return type is int
    auto varFunc = func();
    std::cout<<"The type of varFunc "<<type_info<decltype(varFunc)>()<<std::endl;

//Here the type of auto is int becuase func() is rvalue, and the type of var is int&&
    auto&& varRRFunc = func();
    std::cout<<"The type of varRRFunc "<<type_info<decltype(varRRFunc)>()<<std::endl;

//Here the type of auto is double, but var's type is double& because funcR() is lvalue
    auto&& varRFunc = funcR();
    static_assert(std::is_same_v<decltype(varRFunc), double&>, "Not double&");
    std::cout<<"The type of varRFunc "<<type_info<decltype(varRFunc)>()<<std::endl;

    std::vector<int> vec {1, 7, 3, 0, 8};

//Here the type of auto is vector's iterator to int, and the type of var is const that type for auto with &
    const auto& itVec = vec.begin();
    static_assert(std::is_const_v<std::remove_reference_t<decltype(itVec)>>, "Not const");
    std::cout<<"The type of itVec "<<type_info<decltype(itVec)>()<<std::endl;

    const auto& varCRInt = 1;
    static_assert(std::is_same_v<decltype(varCRInt), const int&>, "Not const int&");

//Here the type of var is std::initializer_list<int> 
    auto var = {1, 8, 65, 8};
    static_assert(std::is_same_v<std::initializer_list<int>, decltype(var)>, "Not same");

}
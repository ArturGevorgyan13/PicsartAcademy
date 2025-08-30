//This task is about to modernize the commented code



// #include <iostream>
// #include <vector>

// typedef std::vector<int> IntVector;

// class ResourceManager {
// private:
//     int* data;

//     int size;

// public:
//     ResourceManager(int n) : size(n) {

//         data = new int[size];
    
//         for(int i = 0; i < size; i++) {

//         data[i] = 0;

//         }

//     }

//     ~ResourceManager() {

//         delete[] data;

//     }

//     ResourceManager(const ResourceManager& other) : size(other.size) {

//         data = new int[size];

//         for(int i = 0; i < size; i++) {

//             data[i] = other.data[i];

//         }

//     }

//     void process() {

//         IntVector vec;

//         vec.push_back(1);

//         vec.push_back(2);

//         vec.push_back(3);

//         for(IntVector::iterator it = vec.begin(); it != vec.end(); ++it) {

//             std::cout << *it << " ";

//         }

//     }

//     virtual void display() {

//         std::cout << "ResourceManager" << std::endl;

//     }

// };

// class DerivedManager : public ResourceManager {
// public:
//     DerivedManager(int n) : ResourceManager(n) {}

//     virtual void display() {

//         std::cout << "DerivedManager" << std::endl;

//     }
// };

#include <iostream>
#include <vector>

//I include memory library for using smart poninters
#include <memory>

//Here I replaced typdef with using
using IntVector = std::vector<int>;

class ResourceManager {
private:
//Here I replace raw pointer to smart pointer
    std::unique_ptr<int[]> data;

    int size;

public:
    ResourceManager(int n) : size(n) {

//Here I use make_unique, because data is smart pointer now
        data = std::make_unique<int[]>(n);
    
        for(int i = 0; i < size; i++) {

            data[i] = 0;

        }

    }


//I replace () with {}
    ResourceManager(const ResourceManager& other) : size{other.size} {

        data = std::make_unique<int[]>(size);

        for(int i = 0; i < size; i++) {

            data[i] = other.data[i];

        }

    }

//Here I implement copy assignment
    ResourceManager& operator=(const ResourceManager& other) {

        if(this != &other) {

            size = other.size;

            data = std::make_unique<int[]>(size);

            for (std::size_t i = 0; i < size; ++i) {

                data[i] = other.data[i];

            }

        }

        return *this;

    }

//Here I implement move ctor, and move assignment
    ResourceManager(ResourceManager&& other) : data{std::move(other.data)}, size{other.size} {}

    ResourceManager& operator=(ResourceManager&& other) {

        if(this != &other) {

            data = std::move(other.data);

            size = other.size;

        }

        return *this;

    }

    void process() {

//Here I use {} to initialize vector with elements
        IntVector vec {1, 2, 3};

//Here I use range-based for instead of classic for
        for(const auto& it: vec) {

            std::cout << it << " ";

        }

    }

    virtual void display() {

        std::cout << "ResourceManager" << std::endl;

    }

};

//Here I add final keyword that other class will not be able to inherit from DerivedManager
class DerivedManager final : public ResourceManager {
public:
    DerivedManager(int n) : ResourceManager{n} {}

//Here I use override keyword because this function is virtual and coming from base class
    void display() override {

        std::cout << "DerivedManager" << std::endl;

    }

};

int main()

{

    ResourceManager var1 {5};

    DerivedManager var2 {8};

    var1.display();

    var1.process();

    var2.display();

    var2.process();

    return 0;

}
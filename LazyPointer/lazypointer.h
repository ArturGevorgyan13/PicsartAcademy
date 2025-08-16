#ifndef L_H
#define L_H

#include <iostream>
#include <memory>
#include <mutex>
#include <tuple>
#include <utility>
#include <functional>

template <typename T>
class lazy_ptr {
private:
    void ensure() const; //Helper function

public:
    //Constructors
    lazy_ptr() = default;

    template <typename ...Args>
    explicit lazy_ptr(Args&&... args);

    //Move constructor, move assignment
    lazy_ptr(lazy_ptr&& other) noexcept;
    lazy_ptr& operator=(lazy_ptr&& other) noexcept;

    //Copy constructor, copy assignment
    lazy_ptr(const lazy_ptr& other) = delete;
    lazy_ptr& operator=(const lazy_ptr& other) = delete;
    
    //Destructors
    ~lazy_ptr() = default;

    //Access operators
    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;
    
    //Utility methods
    T* get();
    const T* get() const;
    bool is_constructed() const noexcept;
    void reset();

    template <typename ...Args>
    void reset(Args&&... args);

    explicit operator bool() const noexcept;

private:
    mutable std::unique_ptr<T> object; //To keep the real object
    mutable std::function<std::unique_ptr<T>()> args; //To keep the arguments from constructor
    mutable std::once_flag flag; //To keep the fact that object is created once
    mutable std::mutex m; //To solve problems with threads
    mutable bool constructed = false; //To have info is object created or not

};

template <typename T>
template <typename ...Args>
lazy_ptr<T>::lazy_ptr(Args&&... args) {

    this->args = [args_tuple = std::make_tuple(std::forward<Args>(args)...)]() mutable {

        return std::apply([](auto&&... a){

            return std::make_unique<T>(std::forward<decltype(a)>(a)...);

        }, args_tuple);

    };

}

template <typename T>
lazy_ptr<T>::lazy_ptr(lazy_ptr&& other) noexcept {

    std::scoped_lock lock(m);

    object = std::move(other.object);

    args = std::move(other.args);

    constructed = other.constructed;

}

template <typename T>
lazy_ptr<T>& lazy_ptr<T>::operator=(lazy_ptr&& other) noexcept{

    if(this != &other) {

        std::scoped_lock lock(m);

        object = std::move(other.object);

        args = std::move(other.args);

        constructed = other.constructed;

    }

    return *this;

}

template <typename T>
T& lazy_ptr<T>::operator*() {

    ensure();

    return *object;

}

template <typename T>
const T& lazy_ptr<T>::operator*() const {

    ensure();

    return *object;

}

template <typename T>
T* lazy_ptr<T>::operator->() {

    ensure();

    return object.get();

}

template <typename T>
const T* lazy_ptr<T>::operator->() const {

    ensure();

    return object.get();

}

template <typename T>
T* lazy_ptr<T>::get() {

    ensure();

    return object.get();

}

template <typename T>
const T* lazy_ptr<T>::get() const {

    ensure();

    return object.get();

}

template <typename T>
bool lazy_ptr<T>::is_constructed() const noexcept {

    return constructed; 
}

template <typename T>
void lazy_ptr<T>::reset() {

    std::scoped_lock lock(m);

    object.reset();

    constructed = false;

}

template <typename T>
template <typename ...Args>
void lazy_ptr<T>::reset(Args&&... arg) {

    std::scoped_lock lock(m);

    object.reset();

    constructed = false;

    args = [args_tuple = std::make_tuple(std::forward<Args>(arg)...)]() mutable {

        return std::apply([](auto&&... a){

            return std::make_unique<T>(std::forward<decltype(a)>(a)...);

        }, args_tuple);
        
    };

}

template <typename T>
lazy_ptr<T>::operator bool() const noexcept {

    return constructed && object.get();

}

template <typename T>
void lazy_ptr<T>::ensure() const {

     std::call_once(flag, [&] {

        if (args) {

            object = args();

            constructed = true;

            args = nullptr;

        }

    });

}

#endif
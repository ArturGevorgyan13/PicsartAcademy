#include <iostream>

template <typename T>
class PoolAllocator{

public:

PoolAllocator(size_t poos_size);

~PoolAllocator();

T* allocate();

void deallocate(T* ptr);

size_t capacity() const;

size_t available() const;

bool is_full() const;

bool is_empty() const;

private:

struct Node{

    Node* next;

};

Node* free;

T* pool;

size_t size;

size_t freeCount;

};

template <typename T>
PoolAllocator<T>::PoolAllocator(size_t poos_size): free{nullptr},pool{nullptr},size{poos_size},freeCount{poos_size}{

    if(poos_size<=0) return;

    pool=static_cast<T*>(::operator new(poos_size*sizeof(T),std::align_val_t(alignof(T))));

    for(int i=0;i<size;++i){

        Node* node=reinterpret_cast<Node*>(pool+i);

        node->next=free;

        free=node;

    }

    std::cout<<"The pool is allocated with "<<size<<" size.\n";

}

template <typename T>
PoolAllocator<T>::~PoolAllocator(){

    for(int i=0;i<size;++i){

        T* ob=pool+i;

        ob->~T();

    }

    ::operator delete(pool,std::align_val_t(alignof(T)));

    std::cout<<"The pool is deallocated with "<<size<<" size.\n";

}

template <typename T>
T* PoolAllocator<T>::allocate(){

    if(!free){

        throw std::bad_alloc();

    }

    Node* tmp=free;

    free=free->next;

    --freeCount;

    std::cout<<"The object is allocated.\n";

    return new(tmp) T();

}

template <typename T>
void PoolAllocator<T>::deallocate(T* ptr){

    if(!ptr) return;

    ptr->~T();

    Node* tmp=reinterpret_cast<Node*>(ptr);

    tmp->next=free;

    free=tmp;

    ++freeCount;

    std::cout<<"The object is deallocated.\n";

}

template <typename T>
size_t PoolAllocator<T>::capacity() const{

    return size;

}

template <typename T>
size_t PoolAllocator<T>::available() const{

    return freeCount;

}

template <typename T>
bool PoolAllocator<T>::is_full() const{

    return freeCount==0;

}

template <typename T>
bool PoolAllocator<T>::is_empty() const{

    return freeCount==size;

}

int main()

{

    PoolAllocator<double> allocator(3);

    double* p1=allocator.allocate();
    double* p2=allocator.allocate();
    double* p3=allocator.allocate();
    
    allocator.deallocate(p1);
    allocator.deallocate(p2);
    allocator.deallocate(p3);
    

    std::cout<<allocator.available()<<std::endl;

    std::cout<<allocator.is_empty()<<std::endl;
    std::cout<<allocator.is_full()<<std::endl;

    std::cout<<allocator.capacity()<<std::endl;

    return 0;

}
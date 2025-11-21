#include <iostream>
#include <memory_resource>
#include <vector>
#include <list>
#include <cstddef>
#include <new>
#include <memory>
#include <cmath>
#include <stdexcept>
#include <concepts>


// РЕАЛИЗОВАТЬ ДИНАМИЧЕСКИЙ МАССИВ С АЛЛОКАТОРОМ ПО ПРИЦИПУ STD::LIST
template <class T>
class Iterator{
private:
    T* ptr;

public:
    //подводка к стандарту STL: переопределения обьектов и операторов
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    Iterator(T* p = nullptr) : ptr(p) {}

    reference operator*() const { return *ptr; }
    pointer operator->() const { return ptr; }

    Iterator& operator++() { ++ptr; return *this; }      
    Iterator operator++(int) { Iterator tmp = *this; ++ptr; return tmp; }

    bool operator==(const Iterator& other) const { return ptr == other.ptr; }
    bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
};


class Allocator: public std::pmr::memory_resource{
public:
    explicit Allocator(size_t memory): buffer(new std::byte[memory]),capacity(memory){
        cells.push_back(Block{buffer,memory,true});
    }
    
    ~Allocator() override {
        delete[] buffer;
    }

    void* do_allocate(size_t bytes, size_t alignment){
        for (auto iter = cells.begin();iter!=cells.end();iter++){
            Block& b = *iter;
            if (b.free==true && b.size >= bytes){
                if (b.size > bytes) {
                    std::byte* newPtr = static_cast<std::byte*>(b.ptr) + bytes;
                    cells.insert(std::next(iter), 
                        Block{ newPtr, b.size - bytes, true });
                }
                b.size = bytes;
                b.free = false;

                return b.ptr;
            }
        }
        throw std::bad_alloc();
    }
    
    void do_deallocate(void* p, size_t bytes, size_t alignment){
        for (auto iter = cells.begin();iter!=cells.end();iter++){
            Block& current = *iter;
            if (current.ptr == p){
                current.free = true;

            }
        }
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

private:
    struct Block{
        void *ptr;
        size_t size;
        bool free;
    };
    std::byte* buffer;
    size_t capacity;
    std::list<Block> cells;
};


template <class T>
class Array{
private:
    std::pmr::polymorphic_allocator<T> alloc;
    T* items = nullptr;
    size_t length = 0;
    size_t capacity = 0;

public:
    using iterator = Iterator<T>;

    iterator begin() { return iterator(items); }
    iterator end() { return iterator(items + length); }


    Array(std::pmr::memory_resource* allocator_type=nullptr):alloc(allocator_type), capacity(5){
        items = alloc.allocate(capacity);
    }


    ~Array(){
        for (size_t i=0; i<length;i++) {
            alloc.destroy(&items[i]);
        }
        if (items) {
            alloc.deallocate(items, capacity);
            items = nullptr;
        }
    }


    void push_back(const T& element) {
    if (length<capacity) {
        alloc.construct(&items[length], element);
        ++length;
    } 

    else {
        size_t new_capacity =capacity * 2;
        T* new_elements = alloc.allocate(new_capacity);

        for (size_t i=0; i<length;++i) {
            alloc.construct(&new_elements[i], items[i]);
            alloc.destroy(&items[i]);
        }

        T* old_items = items;
        items = new_elements;

        alloc.deallocate(old_items, capacity);
        capacity = new_capacity;

        alloc.construct(&items[length], element);
        ++length;
    }
    }

    int size(){return length;}
    int currency_capacity(){return capacity;}

    void pop(size_t index) {
        if (index>=length) {
            throw std::out_of_range("Index out of range");
        }
        T* new_elements = alloc.allocate(length-1);
        size_t cnt=0;
        for (size_t i=0; i<length;++i) {
            if (i ==index) {
                alloc.destroy(&items[i]);
                continue;
            }
            alloc.construct(&new_elements[cnt], items[i]);
            alloc.destroy(&items[i]);
            ++cnt;
        }

        alloc.deallocate(items, length);
        items = new_elements;
        length--;
        }

};


template <typename T>
std::ostream& operator<<(std::ostream& os, Array<T>& arr) {
    os << "[";
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        os << *it;
        if (std::next(it) != arr.end()) os << ", ";
    }
    os << "] - динамический массив, используем аллокатор - Allocator";
    return os;
}





//int main(){
//    Allocator mem(1024);
//    Array<int> arr(&mem);
//    arr.push_back(10);
//    arr.push_back(20);
//    arr.push_back(20);
//    int* pull = new int[2,2,35,1];
//    pull[0] = 1;
//    pull[1] = 2;
//    std::cout<<&pull[1]<<std::endl;
//

//g++ -std=c++20 lab5.cpp -o l
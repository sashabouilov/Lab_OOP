#pragma once
#include <memory>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <initializer_list>


template<typename T>
class Array {
private:
    std::unique_ptr<T[]> data;
    size_t currentSize;
    size_t currentCapacity;
    
    void reallocate(size_t newCapacity) {
        std::unique_ptr<T[]> newData = std::make_unique<T[]>(newCapacity);
        
        for (size_t i = 0; i < currentSize; ++i) {
            newData[i] = std::move(data[i]);
        }
        
        data = std::move(newData);
        currentCapacity = newCapacity;
        std::cout << "Array reallocated to capacity: " << currentCapacity << std::endl;
    }
    

public:
    Array() : currentSize(0), currentCapacity(10) {
        data = std::make_unique<T[]>(currentCapacity);
    }
    

    Array(size_t initialCapacity) : currentSize(0), currentCapacity(initialCapacity) {
        data = std::make_unique<T[]>(currentCapacity);
    }
    

    Array(const std::initializer_list<T>& list) : currentSize(0), currentCapacity(list.size() * 2) {
        data = std::make_unique<T[]>(currentCapacity);
        for (const auto& item : list) {
            push_back(item);
        }
    }
    

    Array(const Array& other) : currentSize(other.currentSize), currentCapacity(other.currentCapacity) {
        data = std::make_unique<T[]>(currentCapacity);
        for (size_t i = 0; i < currentSize; ++i) {
            data[i] = other.data[i];
        }
    }
    

    Array& operator=(const Array& other) {
        if (this != &other) {
            currentSize = other.currentSize;
            currentCapacity = other.currentCapacity;
            data = std::make_unique<T[]>(currentCapacity);
            for (size_t i = 0; i < currentSize; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    

    Array(Array&& other) noexcept 
        : data(std::move(other.data)), 
          currentSize(other.currentSize), 
          currentCapacity(other.currentCapacity) {
        other.currentSize = 0;
        other.currentCapacity = 0;
    }
    
    
    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            currentSize = other.currentSize;
            currentCapacity = other.currentCapacity;
            other.currentSize = 0;
            other.currentCapacity = 0;
        }
        return *this;
    }
    

    void push_back(const T& value) {
        if (currentSize >= currentCapacity) {
            reallocate(currentCapacity * 2);
        }
        data[currentSize++] = value;
    }
    

    void push_back(T&& value) {
        if (currentSize >= currentCapacity) {
            reallocate(currentCapacity * 2);
        }
        data[currentSize++] = std::move(value);
    }
    

    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (currentSize >= currentCapacity) {
            reallocate(currentCapacity * 2);
        }
        data[currentSize++] = T(std::forward<Args>(args)...);
    }
    

    void remove(size_t index) {
        if (index >= currentSize) {
            throw std::out_of_range("Index out of range");
        }
        
        for (size_t i = index; i < currentSize - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        --currentSize;
    }
    

    void pop_back() {
        if (currentSize > 0) {
            --currentSize;
        }
    }
    

    T& operator[](size_t index) {
        if (index >= currentSize) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    

    const T& operator[](size_t index) const {
        if (index >= currentSize) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    

    T& at(size_t index) {
        if (index >= currentSize) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    

    const T& at(size_t index) const {
        if (index >= currentSize) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    

    T& front() {
        if (currentSize == 0) {
            throw std::out_of_range("Array is empty");
        }
        return data[0];
    }
    

    const T& front() const {
        if (currentSize == 0) {
            throw std::out_of_range("Array is empty");
        }
        return data[0];
    }
    

    T& back() {
        if (currentSize == 0) {
            throw std::out_of_range("Array is empty");
        }
        return data[currentSize - 1];
    }
    

    const T& back() const {
        if (currentSize == 0) {
            throw std::out_of_range("Array is empty");
        }
        return data[currentSize - 1];
    }
    

    size_t size() const { return currentSize; }
    size_t capacity() const { return currentCapacity; }
    bool empty() const { return currentSize == 0; }
    

    void clear() {
        currentSize = 0;
    }
    

    void resize_elements(size_t newSize, const T& value = T()) {
        if (newSize > currentCapacity) {
            reallocate(newSize * 2);
        }
        
        if (newSize > currentSize) {
            for (size_t i = currentSize; i < newSize; ++i) {
                data[i] = value;
            }
        }
        currentSize = newSize;
    }
    

    void reserve(size_t newCapacity) {
        if (newCapacity > currentCapacity) {
            reallocate(newCapacity);
        }
    }
    

    T* begin() { return data.get(); }
    T* end() { return data.get() + currentSize; }
    const T* begin() const { return data.get(); }
    const T* end() const { return data.get() + currentSize; }
    const T* cbegin() const { return data.get(); }
    const T* cend() const { return data.get() + currentSize; }
    
    ~Array() noexcept {}
};
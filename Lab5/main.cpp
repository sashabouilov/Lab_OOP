#include <memory_resource>
#include <vector>
#include <memory>
#include <iostream>
#include <cstddef>
#include <iterator>
#include <algorithm>
#include <stdexcept>


class DynamicMemoryResource : public std::pmr::memory_resource {
private:
    struct BlockInfo {
        void* ptr;
        std::size_t size;
    };
    
    BlockInfo* blocks = nullptr;
    std::size_t blocks_count = 0;
    std::size_t blocks_capacity = 0;
    
    void add_block(void* ptr, std::size_t size) {
        if (blocks_count >= blocks_capacity) {
            std::size_t new_capacity = blocks_capacity == 0 ? 16 : blocks_capacity * 2;
            BlockInfo* new_blocks = static_cast<BlockInfo*>(::operator new(
                new_capacity * sizeof(BlockInfo)));
            
            for (std::size_t i = 0; i < blocks_count; ++i) {
                new_blocks[i] = blocks[i];
            }
            
            ::operator delete(blocks);
            blocks = new_blocks;
            blocks_capacity = new_capacity;
        }
        
        blocks[blocks_count++] = BlockInfo{ptr, size};
    }
    
    void remove_block(void* ptr) {
        for (std::size_t i = 0; i < blocks_count; ++i) {
            if (blocks[i].ptr == ptr) {
                blocks[i] = blocks[--blocks_count];
                return;
            }
        }
    }

public:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        void* ptr = ::operator new(bytes, std::align_val_t(alignment));
        add_block(ptr, bytes);
        std::cout << "Allocated " << bytes << " bytes at address " << ptr << std::endl;
        return ptr;
    }
    
    void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override {
        std::cout << "Deallocated " << bytes << " bytes at address " << ptr << std::endl;
        remove_block(ptr);
        ::operator delete(ptr, std::align_val_t(alignment));
    }
    
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

    DynamicMemoryResource() = default;
    
    ~DynamicMemoryResource() override {
        std::cout << "DynamicMemoryResource destructor: releasing " 
                  << blocks_count << " blocks" << std::endl;
        
        for (std::size_t i = 0; i < blocks_count; ++i) {
            ::operator delete(blocks[i].ptr);
        }
        ::operator delete(blocks);
    }
    
    DynamicMemoryResource(const DynamicMemoryResource&) = delete;
    DynamicMemoryResource& operator=(const DynamicMemoryResource&) = delete;
};


template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        
        template<typename... Args>
        Node(Node* nxt, Args&&... args) 
            : data(std::forward<Args>(args)...), next(nxt) {}
    };
    
    Node* top_node = nullptr;
    std::size_t size_ = 0;
    std::pmr::polymorphic_allocator<Node> allocator;

public:
    using value_type = T;
    
    explicit Stack(std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : allocator(mr) {}
    
    Stack(const Stack& other) : allocator(other.allocator) {
        if (!other.empty()) {
            Node* current = other.top_node;
            Stack temp(allocator.resource());
            
            while (current != nullptr) {
                temp.push(current->data);
                current = current->next;
            }
            
            current = temp.top_node;
            while (current != nullptr) {
                push(current->data);
                current = current->next;
            }
        }
    }
    
    Stack(Stack&& other) noexcept 
        : top_node(other.top_node), size_(other.size_), allocator(std::move(other.allocator)) {
        other.top_node = nullptr;
        other.size_ = 0;
    }
    
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            clear();
            allocator = other.allocator;
            
            if (!other.empty()) {
                Node* current = other.top_node;
                Stack temp(allocator.resource());
                
                while (current != nullptr) {
                    temp.push(current->data);
                    current = current->next;
                }
                
                current = temp.top_node;
                while (current != nullptr) {
                    push(current->data);
                    current = current->next;
                }
            }
        }
        return *this;
    }
    
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            clear();
            top_node = other.top_node;
            size_ = other.size_;
            allocator = std::move(other.allocator);
            
            other.top_node = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    ~Stack() {
        clear();
    }
    
    template<typename... Args>
    void emplace(Args&&... args) {
        Node* new_node = allocator.allocate(1);
        try {
            allocator.construct(new_node, top_node, std::forward<Args>(args)...);
            top_node = new_node;
            ++size_;
        } catch (...) {
            allocator.deallocate(new_node, 1);
            throw;
        }
    }
    
    void push(const T& value) {
        emplace(value);
    }
    
    void push(T&& value) {
        emplace(std::move(value));
    }
    
    void pop() {
        if (top_node != nullptr) {
            Node* to_delete = top_node;
            top_node = top_node->next;
            allocator.destroy(to_delete);
            allocator.deallocate(to_delete, 1);
            --size_;
        }
    }
    
    T& top() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return top_node->data;
    }
    
    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return top_node->data;
    }
    
    bool empty() const noexcept {
        return top_node == nullptr;
    }
    
    std::size_t size() const noexcept {
        return size_;
    }
    
    void clear() {
        while (!empty()) {
            pop();
        }
    }
    
    class Iterator {
    private:
        Node* current;
        
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        explicit Iterator(Node* node = nullptr) : current(node) {}
        
        reference operator*() const {
            return current->data;
        }
        
        pointer operator->() const {
            return &current->data;
        }
        
        Iterator& operator++() {
            if (current != nullptr) {
                current = current->next;
            }
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };
    
    Iterator begin() {
        return Iterator(top_node);
    }
    
    Iterator end() {
        return Iterator(nullptr);
    }
    
    class ConstIterator {
    private:
        const Node* current;
        
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        
        explicit ConstIterator(const Node* node = nullptr) : current(node) {}
        
        reference operator*() const {
            return current->data;
        }
        
        pointer operator->() const {
            return &current->data;
        }
        
        ConstIterator& operator++() {
            if (current != nullptr) {
                current = current->next;
            }
            return *this;
        }
        
        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const ConstIterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const ConstIterator& other) const {
            return current != other.current;
        }
    };
    
    ConstIterator begin() const {
        return ConstIterator(top_node);
    }
    
    ConstIterator end() const {
        return ConstIterator(nullptr);
    }
    
    ConstIterator cbegin() const {
        return ConstIterator(top_node);
    }
    
    ConstIterator cend() const {
        return ConstIterator(nullptr);
    }
};


struct ComplexType {
    int id;
    std::string name;
    double value;
    
    ComplexType(int i = 0, std::string n = "", double v = 0.0) 
        : id(i), name(std::move(n)), value(v) {}
    
    friend std::ostream& operator<<(std::ostream& os, const ComplexType& ct) {
        return os << "ComplexType{id=" << ct.id << ", name=\"" << ct.name 
                  << "\", value=" << ct.value << "}";
    }
};


void demonstrate() {
    std::cout << "Demonstration of Stack with DynamicMemoryResource\n\n";
    
    DynamicMemoryResource dmr;
    
    std::cout << "1. Working with simple types (int):\n";
    {
        Stack<int> intStack(&dmr);
        
        for (int i = 1; i <= 5; ++i) {
            intStack.push(i * 10);
        }
        
        std::cout << "Stack after adding 5 elements: ";
        for (auto it = intStack.begin(); it != intStack.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\nStack size: " << intStack.size() << std::endl;
        
        intStack.pop();
        intStack.pop();
        
        std::cout << "Stack after 2 deletions: ";
        for (const auto& item : intStack) {
            std::cout << item << " ";
        }
        std::cout << "\nStack size: " << intStack.size() << std::endl;
    
        intStack.emplace(99);
        std::cout << "After emplace(99): ";
        for (const auto& item : intStack) {
            std::cout << item << " ";
        }
        std::cout << "\nTop element: " << intStack.top() << std::endl;
    }
    
    std::cout << "\n2. Working with complex types (ComplexType):\n";
    {
        Stack<ComplexType> complexStack(&dmr);
        
        complexStack.emplace(1, "First", 1.1);
        complexStack.push(ComplexType(2, "Second", 2.2));
        complexStack.emplace(3, "Third", 3.3);
        
        std::cout << "Stack contents:\n";
        for (const auto& item : complexStack) {
            std::cout << "- " << item << std::endl;
        }
        
        std::cout << "Top element: " << complexStack.top() << std::endl;
        
        complexStack.pop();
        std::cout << "After pop(), top element: " << complexStack.top() << std::endl;
    }
    
    std::cout << "\n3. Iterator testing:\n";
    {
        Stack<std::string> stringStack(&dmr);
        stringStack.push("Hello");
        stringStack.push("World");
        stringStack.push("C++");
        stringStack.push("PMR");
        
        std::cout << "Forward traversal: ";
        for (auto it = stringStack.begin(); it != stringStack.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Range-based for: ";
        for (const auto& str : stringStack) {
            std::cout << str << " ";
        }
        std::cout << std::endl;
        
        auto it1 = stringStack.begin();
        auto it2 = stringStack.begin();
        ++it2;
        
        std::cout << "Iterator comparison: ";
        std::cout << "it1 == it2: " << (it1 == it2) << ", ";
        std::cout << "it1 != it2: " << (it1 != it2) << std::endl;
    }
    
    std::cout << "\n4. Copy and move testing:\n";
    {
        Stack<int> original(&dmr);
        original.push(100);
        original.push(200);
        original.push(300);
        
        Stack<int> copy = original;
        std::cout << "Original after copying: ";
        for (const auto& item : original) {
            std::cout << item << " ";
        }
        std::cout << "\nCopy: ";
        for (const auto& item : copy) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
        
        Stack<int> moved = std::move(original);
        std::cout << "Moved stack: ";
        for (const auto& item : moved) {
            std::cout << item << " ";
        }
        std::cout << "\nOriginal is empty after move: " << original.empty() << std::endl;
    }
    
    std::cout << "\nDemonstration completed\n";
}

int main() {
    try {
        demonstrate();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
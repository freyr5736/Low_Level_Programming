#include "myiterator.hpp"
#include <cstddef>
#include <iostream>
#include <new>       // for placement new
#include <stdexcept> // for std::out_of_range
#include <utility>   // for std::move

template <typename T> struct cqueue {
    T *data;
    size_t size;
    size_t capacity;
    size_t start;
    using iterator = myiterator<T>; // Define iterator type
    using const_iterator =
        myiterator<const T>; // Define iterator type for const
    // Default constructor
    cqueue() : data(nullptr), size(0), capacity(0), start(0) {}

    // Copy constructor
    cqueue(const cqueue &other)
        : data(nullptr), size(0), capacity(0), start(0) {
        if (other.size == 0)
            return;

        allocate(other.capacity);

        // Copy elements in logical order
        for (size_t i = 0; i < other.size; ++i) {
            size_t idx = (other.start + i) % other.capacity;
            new (data + i) T(other.data[idx]); // copy construct
        }

        size = other.size;
        capacity = other.capacity;
        start = 0; // reset to 0 in new buffer
    }

    // Allocate memory for obj elements
    void allocate(size_t obj) {
        data = static_cast<T *>(::operator new(obj * sizeof(T)));
        capacity = obj;
    }

    // Deallocate memory
    void deallocate(T *dataptr) { ::operator delete(dataptr); }

    // Add to rear of queue
    void cadd(const T &value) {
        if (size == capacity) {
            size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
            T *new_data =
                static_cast<T *>(::operator new(new_capacity * sizeof(T)));

            // Move old elements to new buffer (in order)
            for (size_t i = 0; i < size; ++i) {
                size_t idx = (start + i) % capacity;
                new (new_data + i) T(std::move(data[idx]));
                data[idx].~T(); // destroy old
            }

            ::operator delete(data);
            data = new_data;
            capacity = new_capacity;
            start = 0;
        }

        size_t idx = (start + size) % capacity;
        new (data + idx) T(value); // placement new
        ++size;
    }

    // Remove front
    void cpop() {
        if (size == 0)
            return;
        data[start].~T(); // destroy
        start = (start + 1) % capacity;
        --size;
    }

    // Access front
    T &front() {
        if (size == 0)
            throw std::out_of_range("Queue is empty");
        return data[start];
    }

    T &back() {
        if (size == 0)
            throw std::out_of_range("Queue is empty");
        size_t idx = (start + size - 1) % capacity;
        return data[idx];
    }

    // Queue is empty?
    bool empty() const { return size == 0; }

    // Number of elements
    size_t length() const { return size; }

    // Clear queue
    void clear() {
        for (size_t i = 0; i < size; ++i)
            data[(start + i) % capacity].~T();
        size = 0;
        start = 0;
    }
    // begin(): returns iterator to first element
    iterator begin() { return iterator(data); }

    // end(): returns iterator to one-past-last element
    iterator end() { return iterator(data + size); }

    const_iterator begin() const { return const_iterator(data); }
    const_iterator end() const { return const_iterator(data + size); }

    // Destructor
    ~cqueue() {
        for (size_t i = 0; i < size; ++i) {
            size_t idx = (start + i) % capacity;
            data[idx].~T(); // proper destruction
        }
        deallocate(data);
    }
};

int main() {
    std::cout << "===== BEGIN TEST CASES =====\n\n";

    cqueue<int> q;

    // Test 1: Add Elements
    std::cout << "Test 1: Add Elements\n";
    q.cadd(10);
    q.cadd(20);
    q.cadd(30);
    std::cout << "Expected Front: 10, Back: 30\nGot:      Front: " << q.front()
              << ", Back: " << q.back() << "\n\n";

    // Test 2: Pop Element
    std::cout << "Test 2: Pop Element\n";
    q.cpop(); // removes 10
    std::cout << "Expected Front: 20\nGot:      Front: " << q.front() << "\n\n";

    // Test 3: Add More (Test Wrap-around)
    std::cout << "Test 3: Add More (Test Wrap-around)\n";
    q.cadd(40);
    q.cadd(50);
    std::cout << "Expected Front: 20, Back: 50\nGot:      Front: " << q.front()
              << ", Back: " << q.back() << "\n\n";

    // Test 4: Pop All Elements
    std::cout << "Test 4: Pop All Elements\n";
    q.cpop(); // removes 20
    q.cpop(); // removes 30
    q.cpop(); // removes 40
    q.cpop(); // removes 50
    std::cout << "Expected: Queue Empty\nGot:      "
              << (q.empty() ? "Queue Empty" : "Not Empty") << "\n\n";

    // Test 5: Add After Empty (Reuse after Clear)
    std::cout << "Test 5: Add After Empty\n";
    q.cadd(99);
    q.cadd(77);
    std::cout << "Expected Front: 99, Back: 77\nGot:      Front: " << q.front()
              << ", Back: " << q.back() << "\n\n";

    // Test 6: Clear
    std::cout << "Test 6: Clear\n";
    q.clear();
    std::cout << "Expected: Queue Empty\nGot:      "
              << (q.empty() ? "Queue Empty" : "Not Empty") << "\n\n";

    // Test 7: Copy Constructor
    std::cout << "Test 7: Copy Constructor\n";
    q.cadd(1);
    q.cadd(2);
    q.cadd(3);
    cqueue<int> copy_q = q; // Use copy constructor
    std::cout << "Expected Copy Front: 1, Back: 3\nGot:      Front: "
              << copy_q.front() << ", Back: " << copy_q.back() << "\n\n";

    // Test 8: Iterator Forward Traversal
    std::cout << "Test 8: Iterator Forward Traversal\n";
    std::cout << "Expected: 1 2 3\nGot:      ";
    for (cqueue<int>::iterator it = q.begin(); it != q.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n\n";

    // Test 9: Range-Based For Loop
    std::cout << "Test 9: Range-Based For Loop\n";
    std::cout << "Expected: 1 2 3\nGot:      ";
    for (const int &x : q) {
        std::cout << x << " ";
    }
    std::cout << "\n\n";

    // Test 10: Iterator Arithmetic
    std::cout << "Test 10: Iterator Arithmetic\n";
    auto it = q.begin();
    std::cout << "Expected: *(it+2) = 3\nGot:      *(it+2) = " << *(it + 2)
              << "\n\n";

    // Test 11: Pop and Wrap-Around Behavior
    std::cout << "Test 11: Pop and Wrap-Around Behavior\n";
    q.cpop();  // removes 1
    q.cpop();  // removes 2
    q.cadd(4); // wrap around to index 0
    q.cadd(5);
    std::cout << "Expected Front: 3, Back: 5\nGot:      Front: " << q.front()
              << ", Back: " << q.back() << "\n\n";

    std::cout << "===== END TEST CASES =====\n";

    return 0;
}

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
    std::cout << "Test 1: Basic push and front\n";
    cqueue<int> q1;
    q1.cadd(10);
    q1.cadd(20);
    q1.cadd(30);
    std::cout << "Front: " << q1.front() << "\n";   // Expected: 10
    std::cout << "Back : " << q1.back() << "\n"; // Expected : 30
    std::cout << "Length: " << q1.length() << "\n"; // Expected: 3

    std::cout << "\nTest 2: Pop elements and check order\n";
    q1.cpop();
    std::cout << "Front after pop: " << q1.front() << "\n"; // Expected: 20
    q1.cpop();
    std::cout << "Front after second pop: " << q1.front()
              << "\n"; // Expected: 30
    q1.cpop();
    std::cout << "Empty after popping all: " << (q1.empty() ? "Yes" : "No")
              << "\n"; // Expected: Yes

    std::cout << "\nTest 3: Circular behavior check\n";
    for (int i = 1; i <= 5; ++i)
        q1.cadd(i * 10); // 10,20,30,40,50

    q1.cpop(); // remove 10
    q1.cpop(); // remove 20
    q1.cadd(60);
    q1.cadd(70); // triggers wrap-around
    while (!q1.empty()) {
        std::cout << q1.front() << " ";
        q1.cpop();
    }
    std::cout << "\n"; // Expected order: 30 40 50 60 70

    std::cout << "\nTest 4: Clear and reuse\n";
    q1.cadd(100);
    q1.cadd(200);
    q1.clear();
    std::cout << "Length after clear: " << q1.length() << "\n"; // Expected: 0
    std::cout << "Empty? " << (q1.empty() ? "Yes" : "No")
              << "\n"; // Expected: Yes
    q1.cadd(300);
    std::cout << "Front after reuse: " << q1.front() << "\n"; // Expected: 300

    std::cout << "\nTest 5: Copy constructor\n";
    q1.cadd(400);
    cqueue<int> q2 = q1;                                      // Copy
    std::cout << "Front of original: " << q1.front() << "\n"; // Expected: 300
    std::cout << "Front of copy: " << q2.front() << "\n";     // Expected: 300
    q2.cpop();
    std::cout << "After pop on copy, front: " << q2.front()
              << "\n"; // Expected: 400
    std::cout << "Original still intact: " << q1.front()
              << "\n"; // Expected: 300

    std::cout << "\nAll tests completed successfully.\n";

    return 0;
}

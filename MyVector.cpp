#include <cstddef>  // for size_t
#include <cstring>  // optional, for memcpy
#include <iostream> // optional, for debugging

template <typename T> struct myvector {
    T *data;         // Pointer to dynamically allocated array of T
    size_t size;     // Number of elements currently used
    size_t capacity; // Total allocated slots (not necessarily filled)

    // Default constructor
    myvector() : data(nullptr), size(0), capacity(0) {}

    // Template copy constructor to allow conversion like myvector<int> to
    // myvector<double>, etc.
    template <typename U> myvector(const myvector<U> &other) {}

    // Allocate memory
    void allocate(size_t obj) {
        data = static_cast<T *>(
            ::operator new(obj * sizeof(T))); // raw allocation (uninitialized)
        capacity = obj;
    }

    // Deallocate mempory
    void deallocate(T *dataptr) { ::operator delete(dataptr); }

    void mypush(const T &value) {
        if (size == capacity) {
            size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;

            // Allocate new memory
            T *new_data =
                static_cast<T *>(::operator new(new_capacity * sizeof(T)));

            // Move or copy existing elements to new_data
            for (size_t i = 0; i < size; ++i) {
                // new(...) T(...) is placement new
                new (new_data + i)
                    T(std::move(data[i])); // move-construct: At memory address
                                           // new_data + i, construct a T object
                                           // using the moved value of data[i]
                data[i].~T();              // explicitly call destructor
            }

            // Deallocate old memory
            ::operator delete(data);

            data = new_data;
            capacity = new_capacity;
        }
        // Placement new for the new element
        new (data + size) T(value);

        ++size;
    }

    // Destructor to release memory
    ~myvector() {
        for (size_t i = 0; i < size; ++i) {
            data[i].~T(); // manually destroy each object
        }
        deallocate(data);
    }
};

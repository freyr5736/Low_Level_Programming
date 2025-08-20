// ------------------------------------- Placement new IMPLEMENTATION  -------------------------------------
#include <iostream>
#include <new>  // For placement new

template<typename T>
class arena_allocator {
    char* buffer;
    size_t capacity;
    size_t offset;

public:
    arena_allocator(size_t count) : capacity(count * sizeof(T)), offset(0) {
        buffer = new char[capacity];  // Allocate raw memory block
    }

    // Allocates memory and constructs an object of type T using placement new
    template<typename... Args>
    
    T* allocate(Args&&... args) {
        if (offset + sizeof(T) > capacity)
            return nullptr;  // Out of space
        
        // Calculate where to place the next object
        T* ptr = reinterpret_cast<T*>(buffer + offset);

        // Construct the object in-place using placement new with forwarded arguments
        new (ptr) T(std::forward<Args>(args)...);

        offset += sizeof(T);  // Move offset forward
        return ptr;
    }

    // Reset allocator (does NOT call destructors)
    void reset() {
        offset = 0;
    }

    ~arena_allocator() {
        // Call destructors manually for all constructed objects if needed (not handled here)
        delete[] buffer;
    }
};




// ------------------------------------- BASIC IMPLEMENTATION  -------------------------------------
// #include <iostream>

// template<typename T>
// class arena_allocator {
//     char* buffer;     // Raw memory buffer to store objects of type T.
//     size_t capacity;  // Total size of the buffer in bytes.
//     size_t offset;    // Current offset in the buffer (how much memory has been allocated so far).

// public:
//     // Constructor: Allocate raw memory sufficient to hold 'count' objects of type T.
//     arena_allocator(size_t count) : capacity(count * sizeof(T)), offset(0) {
//         buffer = new char[capacity];  // Allocate a continuous block of raw bytes.
//     }

//     // Allocates memory for one object of type T from the arena.
//     T* allocate() {
//         // Check if there’s enough space left in the buffer to allocate another object of type T.
//         if (offset + sizeof(T) > capacity)
//             return nullptr;  // Allocation fails if buffer is exhausted.

//         // Calculate the address where the next object should be allocated.
//         T* ptr = reinterpret_cast<T*>(buffer + offset);

//         // Move the offset forward by sizeof(T) to prepare for the next allocation.
//         offset += sizeof(T);

//         // Return pointer to the newly allocated memory region for T.
//         return ptr;
//     }

//     // Resets the allocator to reuse the buffer from the start (very cheap "deallocation").
//     void reset() {
//         offset = 0;  // Simply resets the offset; no actual deallocation happens.
//     }

//     // Destructor: Free the raw memory buffer.
//     ~arena_allocator() {
//         delete[] buffer;
//     }
// };



// // Simple POD struct to test allocation
// struct Point {
//     int x, y;
// };

// int main() {
//     // Create an arena allocator for 5 Point objects
//     arena_allocator<Point> allocator(5);

//     // Array to hold allocated Point pointers
//     Point* points[5];

//     // Allocate and initialize 5 Points
//     for (int i = 0; i < 5; ++i) {
//         points[i] = allocator.allocate();
//         if (points[i]) {
//             points[i]->x = i;
//             points[i]->y = i * 10;
//         } else {
//             std::cout << "Allocation failed at index " << i << std::endl;
//         }
//     }

//     // Print allocated Points to verify
//     for (int i = 0; i < 5; ++i) {
//         std::cout << "Point " << i << ": (" << points[i]->x << ", " << points[i]->y << ")\n";
//     }

//     // Attempt to allocate beyond capacity (should fail and return nullptr)
//     Point* fail_alloc = allocator.allocate();
//     if (fail_alloc == nullptr) {
//         std::cout << "Allocation failed as expected when buffer is full.\n";
//     }

//     // Reset allocator and allocate again
//     allocator.reset();
//     Point* p0 = allocator.allocate();
//     if (p0) {
//         p0->x = 100;
//         p0->y = 200;
//         std::cout << "After reset: (" << p0->x << ", " << p0->y << ")\n";
//     }

//     return 0;
// }


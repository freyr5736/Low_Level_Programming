#include <iostream>     
#include <memory>      
#include <vector>      

// ==========================
// Custom Allocator Template
// ==========================

template<class T>
class TrackingAllocator
{
public:
    // Standard typedefs required by allocator-aware containers (like std::vector)
    using value_type = T;                    // The type this allocator allocates
    using pointer = T*;                      // Pointer to T
    using const_pointer = const T*;          // Pointer to const T
    using size_type = size_t;               // Type used for sizes

    // Default constructor — no initialization needed
    TrackingAllocator() = default;

    // Copy constructor template: allows copying from allocator of a different type U
    // Required for allocator rebind in containers like std::vector
    template<class U>
    TrackingAllocator(const TrackingAllocator<U> &other) {}

    // Destructor — nothing specific to clean up
    ~TrackingAllocator() = default;

    // Allocation function used by STL containers (e.g., vector)
    pointer allocate(size_type numObjects)
    {
        // Increase the static allocation counter by number of elements allocated
        mAllocations += numObjects;

        // Allocate raw uninitialized memory for numObjects of type T
        // operator new returns a void*, which is cast to T*
        return static_cast<pointer>(operator new(sizeof(T) * numObjects));
    }

    // Deallocation function — releases the raw memory allocated earlier
    void deallocate(pointer p, size_type numObjects)
    {
        // Standard operator delete used to free the memory
        // numObjects is unused here, but is part of the allocator interface
        operator delete(p);
    }

    // Accessor to the current number of allocations made
    size_type get_allocations() const
    {
        return mAllocations;
    }

private:
    // Static counter shared across all instances of TrackingAllocator<T>
    // This allows us to count total allocations for the given type T
    static size_type mAllocations;
};

// Define the static member outside the class
template<class T>
typename TrackingAllocator<T>::size_type TrackingAllocator<T>::mAllocations = 0;

int main()
{
    // Create a std::vector of 5 integers using our custom allocator
    std::vector<int, TrackingAllocator<int>> v(5);
    // At this point, the vector constructor will:
    // - Default-initialize the allocator
    // - Use it to allocate memory for 5 integers

    // get_allocator() returns a copy of the allocator used internally
    // get_allocations() returns the static counter for how many elements were allocated
    std::cout << v.get_allocator().get_allocations() << std::endl;

    return 0;
}

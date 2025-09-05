#include <cstddef>  // for size_t
#include <cstring>  // optional, for memcpy
#include <iostream> // optional, for debugging

template <typename T> struct myiterator {
    T *ptr;

    // Constructor
    myiterator(T *p = nullptr) : ptr(p) {}

    // Pre-increment (++it)
    myiterator &operator++() {
        ++ptr;
        return *this;
    }

    // Post-increment (it++)
    myiterator operator++(int) {
        myiterator temp = *this;
        ++ptr;
        return temp;
    }

    // Pre-decrement (--it)
    myiterator &operator--() {
        --ptr;
        return *this;
    }

    // Post-decrement (it--)
    myiterator operator--(int) {
        myiterator temp = *this;
        --ptr;
        return temp;
    }

    // Dereference
    T &operator*() const { return *ptr; }

    // Equality
    bool operator==(const myiterator &other) const { return ptr == other.ptr; }

    // Inequality
    bool operator!=(const myiterator &other) const {
        return !(ptr == other.ptr);
    }

    // Move forward n positions
    myiterator operator+(int n) const { return myiterator(ptr + n); }

    // Move backward n positions
    myiterator operator-(int n) const { return myiterator(ptr - n); }

    // Distance between iterators
    int operator-(const myiterator &other) const { return ptr - other.ptr; }

    T &operator[](int index) const { return *(ptr + index); }
};

template <typename T> struct myvector {
    T *data;         // Pointer to dynamically allocated array of T
    size_t size;     // Number of elements currently used
    size_t capacity; // Total allocated slots (not necessarily filled)
    using iterator = myiterator<T>; // Define iterator type
    using const_iterator =
        myiterator<const T>; // Define iterator type for const

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

    // Pop the last element
    void mypop() {
        if (size == 0)
            return; // avoid underflow

        // Call destructor for last element
        data[size - 1].~T();
        --size;
    }

    // General index access

    // for non constant objects (can modify elements)
    T &operator[](size_t index) { return data[index]; }

    // for constant objects (read only access)
    const T &operator[](size_t index) const { return data[index]; }

    // for printing the vector
    void myprint() {
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i] << " ";
        }
    }

    // Using quick sort for sorting
    void quicksort(size_t low, size_t high, bool descending) {
        if (low >= high)
            return;

        size_t pivotIndex = partition(low, high, descending);
        if (pivotIndex > 0)
            quicksort(low, pivotIndex - 1, descending);
        quicksort(pivotIndex + 1, high, descending);
    }

    size_t partition(size_t low, size_t high, bool descending) {
        T &pivot = data[high];
        size_t i = low;

        for (size_t j = low; j < high; ++j) {
            bool condition = descending ? (data[j] > pivot) : (data[j] < pivot);
            if (condition) {
                std::swap(data[i], data[j]);
                ++i;
            }
        }
        std::swap(data[i], data[high]);
        return i;
    }

    void mysort(bool descending = false) {
        if (size > 1)
            quicksort(0, size - 1, descending);
    }

    // manual clearing
    void clear() {
        for (size_t i = 0; i < size; ++i)
            data[i].~T();
        size = 0;
    }

    // begin(): returns iterator to first element
    iterator begin() { return iterator(data); }

    // end(): returns iterator to one-past-last element
    iterator end() { return iterator(data + size); }

    const_iterator begin() const { return const_iterator(data); }
    const_iterator end() const { return const_iterator(data + size); }

    // Destructor to release memory
    ~myvector() {
        for (size_t i = 0; i < size; ++i) {
            data[i].~T(); // manually destroy each object
        }
        deallocate(data);
    }
};

int main() {
    std::cout << "===== BEGIN TEST CASES =====\n\n";

    myvector<int> v;

    // Test 1: Push Elements
    std::cout << "Test 1: Push Elements\n";
    v.mypush(5);
    v.mypush(2);
    v.mypush(8);
    v.mypush(1);
    std::cout << "Expected: 5 2 8 1\nGot:      ";
    v.myprint();
    std::cout << "\n\n";

    // Test 2: Index Access
    std::cout << "Test 2: Index Access\n";
    std::cout << "Expected: v[0] = 5, v[2] = 8\nGot:      v[0] = " << v[0] << ", v[2] = " << v[2] << "\n\n";

    // Test 3: Pop Last Element
    std::cout << "Test 3: Pop Last Element\n";
    v.mypop(); // removes 1
    std::cout << "Expected: 5 2 8\nGot:      ";
    v.myprint();
    std::cout << "\n\n";

    // Test 4: Sort Ascending
    std::cout << "Test 4: Sort Ascending\n";
    v.mysort();
    std::cout << "Expected: 2 5 8\nGot:      ";
    v.myprint();
    std::cout << "\n\n";

    // Test 5: Sort Descending
    std::cout << "Test 5: Sort Descending\n";
    v.mysort(true);
    std::cout << "Expected: 8 5 2\nGot:      ";
    v.myprint();
    std::cout << "\n\n";

    // Test 6: Clear
    std::cout << "Test 6: Clear\n";
    v.clear();
    std::cout << "Expected: <empty>\nGot:      ";
    v.myprint();
    std::cout << "\n\n";

    // Test 7: Reuse After Clear
    std::cout << "Test 7: Reuse After Clear\n";
    v.mypush(9);
    v.mypush(1);
    v.mypush(6);
    std::cout << "Expected: 9 1 6\nGot:      ";
    v.myprint();
    std::cout << "\n\n";

    // Test 8: Iterator Forward Traversal
    std::cout << "Test 8: Iterator Forward Traversal\n";
    std::cout << "Expected: 9 1 6\nGot:      ";
    for (myvector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n\n";

    // Test 9: Range-Based For Loop
    std::cout << "Test 9: Range-Based For Loop\n";
    std::cout << "Expected: 9 1 6\nGot:      ";
    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << "\n\n";

    // Test 10: Iterator Arithmetic
    std::cout << "Test 10: Iterator Arithmetic\n";
    auto it = v.begin();
    std::cout << "Expected: *(it+2) = 6\nGot:      *(it+2) = " << *(it + 2) << "\n\n";

    // Test 11: Full Iterator Loop
    std::cout << "Test 11: Full Iterator Loop\n";
    std::cout << "Expected: 9 1 6\nGot:      ";
    for (myvector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n\n";

    std::cout << "===== END TEST CASES =====\n";

    return 0;
}


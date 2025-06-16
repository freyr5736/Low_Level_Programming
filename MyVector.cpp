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

    // Destructor to release memory
    ~myvector() {
        for (size_t i = 0; i < size; ++i) {
            data[i].~T(); // manually destroy each object
        }
        deallocate(data);
    }
};


int main() {
    myvector<int> v;

    std::cout << "Test 1: Push Elements\n";
    v.mypush(5);
    v.mypush(2);
    v.mypush(8);
    v.mypush(1);
    std::cout << "Current Vector: ";
    v.myprint(); // Should be: 5 2 8 1
    std::cout << "\n";

    std::cout << "\nTest 2: Sort Ascending\n";
    v.mysort(); // ascending
    std::cout << "Sorted Ascending: ";
    v.myprint(); // Should be: 1 2 5 8
    std::cout << "\n";

    std::cout << "\nTest 3: Sort Descending\n";
    v.mysort(true); // descending
    std::cout << "Sorted Descending: ";
    v.myprint(); // Should be: 8 5 2 1
    std::cout << "\n";

    std::cout << "\nTest 4: Pop Last Element\n";
    v.mypop(); // Should remove 1
    std::cout << "After pop: ";
    v.myprint(); // Should be: 8 5 2
    std::cout << "\n";

    std::cout << "\nTest 5: Clear the Vector\n";
    v.clear();
    std::cout << "After clear: ";
    v.myprint(); // Should print nothing
    std::cout << "\n";

    std::cout << "\nTest 6: Push After Clear\n";
    v.mypush(10);
    v.mypush(20);
    std::cout << "After pushing 10 and 20: ";
    v.myprint(); // Should be: 10 20
    std::cout << "\n";

    std::cout << "\nTest 7: Clear Again and Push More\n";
    v.clear();
    v.mypush(7);
    v.mypush(3);
    v.mypush(9);
    std::cout << "After clear and push 7 3 9: ";
    v.myprint(); // Should be: 7 3 9
    std::cout << "\n";

    std::cout << "\nTest 8: Final Sort Ascending\n";
    v.mysort();
    std::cout << "Final Sorted Ascending: ";
    v.myprint(); // Should be: 3 7 9
    std::cout << "\n";

    return 0;
}

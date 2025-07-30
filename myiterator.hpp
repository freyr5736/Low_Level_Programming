template <typename I> struct myiterator {
    I *ptr;

    // Constructor
    myiterator(I *p = nullptr)
        : ptr(p) {} // default value is NULL and passed value will be p

    /*
        in PRE operations, no need to create a temporary copy. You’re just modifying and returning the same object. 
    */
    
    //Pre-increment (++it)
    myiterator &operator++() { // Return by reference (&)
        ++ptr; // returns a reference to the object itself, allowing further operations.
        return *this;
    }

    // Pre-decrement (--it)
    myiterator &operator--() {
        --ptr;
        return *this;
    }

    /*
        in POST operations, Post-increment must return the original value before incrementing hence need a copy of orignal value which we return
        Dummy int parameter to differentiate from PRE operations
    */
    
    // Post-increment (it++)
    myiterator operator++(int){
        myiterator original_value = *this; // current value (original)
        ++ptr; // increases the value
        return original_value; //returns the orignal value
    }

    // Post-decrement (it--)
    myiterator operator--(int){
        myiterator original_value = *this; // current value (original)
        --ptr;  // incrases the value
        return original_value; // returns the original value
    }

    // Dereference (*it)
    I &operator*() const {
        return *ptr;
    }

    // Equality operation (it == other)
    bool operator==(const myiterator &other) const {
        return ptr==other.ptr;
    }

    // Inequality (it != other)
    bool operator!=(const myiterator &other) const {
        return !(ptr == other.ptr);
    }

    /*
    These operators do not modify the current iterator object.
    They return a new iterator, but the original iterator remains unchanged.
    Marking them const:
        Signals intent (non-mutating).
        Allows usage on const objects.
        Enforces const-correctness.
        Move forward in position (it + n)
    */
    myiterator operator+(int n) const {
        return myiterator(ptr+n); // calling the constructor to update ptr
    }

    // Move backward in position (it - n)
    myiterator operator-(int n) const {
        return myiterator(ptr-n);
    }

    // Distance between iterators
    int operator-(const myiterator &other) const {
        return ptr-other.ptr;
    }
    
    // Subscript operator (it[n])
    I &operator[](int index) const { 
        return *(ptr + index); 
    }

};
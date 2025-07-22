#include <iostream>

// By Object Slicing / Static dispatch
// This is unsafe unless you’re absolutely sure of the actual type.
// Compiler won’t generate dynamic type checks here.

class vBase {
  public:
    virtual void print() { std::cout << "Base Greetings!\n"; }
};

class Derived : public vBase {
  public:
    void print() { std::cout << "Derived Greetings!\n"; }
};

void staticPrint(vBase &obj) {
    // Devirtualization: static_cast to derived
    static_cast<Derived &>(obj).print();
}

// By Function Pointers
// Entirely C-style memory layout — fits well in embedded systems.

struct Animal {
    void (*speak)(Animal *); // function pointers -> this is a pointer to a
                             // function that takes a pointer to an Animal
};

void dogSpeak(Animal *) { std::cout << "BARK BARK!\n"; }

void catSpeak(Animal *) { std::cout << "MEOW MEOW!\n"; }

int main() {
    Derived d;
    staticPrint(d); // Calls Derived::print without vtable

    Animal dog{dogSpeak};
    Animal cat{catSpeak};

    // The function is looked up through the function pointer stored in the struct
    dog.speak(&dog);
    cat.speak(&cat);

    return 0;
}

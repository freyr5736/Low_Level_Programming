#include <iostream>

template <typename Derived> class Base {    //2
  public:
    void print() { static_cast<Derived *>(this)->printing(); } //3
};

class Derived : public Base<Derived> {
  public:
    void printing() { std::cout << "used CRTP . . .\n"; } //4
};

int main() {
    Derived obj;
    obj.print(); //1
    return 0;
}
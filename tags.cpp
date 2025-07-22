#include <iostream>

class Tag1 {};
class Tag2{};

void print(int a, Tag1){
    std::cout<<"Used Tag1 "<<a<<std::endl;
}

void print(int a, Tag2){
    std::cout<<"Used Tag2 "<<a<<std::endl;
}

int main(){
    print(20,Tag1{});
    print(30,Tag2{});
    return 0;
}
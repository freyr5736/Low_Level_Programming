#include <thread>
#include <iostream>

using namespace std;


// // Function pointer (basic thread creation)
// void func (int x){
//     while(x--)
//         cout<<x<<endl;
// }

// int main (){
//     thread t1(func,11);
//     t1.join();
//     return 0;
// }


// Lmabda Function
// int main(){
//     // Method 1
//     // auto func = [&] (int x){
//     //     while(x--) cout<<x<<endl;
//     // };
//     // thread t1(func,11);

//     // Method 2
//     thread t1([&](int x){while(x--) cout<<x<<endl;}, 11);
//     t1.join();

//     return 0;
// }


// Functor (Function Object)
// class Base{
//     public:
//         void operator() (int x){ // overloading ()
//             while(x--) cout<<x<<endl;
//         }
// };

// int main(){
//     thread t1(Base(),11); // passing functor
//     t1.join();
//     return 0;
// }


// Non static member function
// class Base{
//     public:
//         void run(int x){
//             while(x--) cout<<x<<endl;
//         }
// };

// int main(){
//     Base b;
//     thread t1(&Base::run, &b, 11);
//     t1.join();
//     return 0;
// }


// Static member fucntion
class Base{
    public:
        static void run(int x){
            while(x--) cout<<x<<endl;
        }
};

int main(){
    Base b;
    thread t1(&Base::run, 11);
    t1.join();
    return 0;
}
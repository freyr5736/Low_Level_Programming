#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;

mutex m;
int buffer = 0;

// // No locking strategy
// void task(const char* thread_number, int loop_for){
//     unique_lock<mutex> lock (m); // automatically calls lock on mutex m
//     for(int i=0;i<loop_for;++i){
//         buffer++;
//         cout<<"Thread "<<thread_number<<" : "<<buffer<<endl;
//     }
// }

// defer lock strategy 
// provides more flexibility to when we want to lock it
void task(const char* thread_number, int loop_for){
    unique_lock<mutex> lock (m,defer_lock); // does not call lock on mutext m
    lock.lock(); // have to explicitly lock mutex m1. is unlocked in destructor
    for(int i=0;i<loop_for;++i){
        buffer++;
        cout<<"Thread "<<thread_number<<" : "<<buffer<<endl;
    }
}


int main(){
    thread t1(task,"t1",10);
    thread t2(task,"t2",10);
    t1.join();
    t2.join();
    return 0;
}
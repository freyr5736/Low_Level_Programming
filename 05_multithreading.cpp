#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int counter = 0;

mutex m;
void incr() {

    for (int i = 0; i < 1000000; ++i) {
        
        // if lock is already in use by one of the threads then the other thread won't enter this if-block, instead it will move to next increament
        if(m.try_lock()){ // TRYING to lock the mutex
            ++counter;
            m.unlock();
        }
    }
  
}

int main() {
    thread t1(incr);
    thread t2(incr);
    t1.join();
    t2.join();
    cout << counter << '\n'; // give different outputs because of try_lock
    return 0;
}

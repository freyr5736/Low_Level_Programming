#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int amt = 0;

// gives different output each time due to race condition
// void add_money() {
//     for (int i = 0; i < 1000000; ++i) {
//         ++amt;
//     }
// }

// using mutex (mutual exclusion) locks solves the race condition. Gives correct output each time.
mutex m;
void add_money() {
    m.lock();
    for (int i = 0; i < 1000000; ++i) {
        ++amt;
    }
    m.unlock();
}

int main() {
    thread t1(add_money);
    thread t2(add_money);
    t1.join();
    t2.join();
    cout << amt << '\n';
    return 0;
}

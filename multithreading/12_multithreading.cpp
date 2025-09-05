#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

mutex m1, m2;

/*
    Scenario 01 : No Deadlock
    lock(m1,m2);        lock(m1,m2);

    Scenario 02 : No Deadlock
    lock(m1,m2);        lock(m2,m1);

    Scenario 03 : No Deadlock
    lock(m1,m2,m3,m4);      lock(m3,m4);
                            lock(m1,m2);

    Scenario 04 : Deadlock Arises
    lock(m3,m4);        lock(m1,m2);
    lock(m1,m2);        lock(m3,m4);

*/


void task_01() {
    long long t1_counter = 0;
    while (true) {
        lock(m1, m2);
        //m1.lock();
        //m2.lock();
        cout << "take 1 : "<<t1_counter++<<endl;
        m1.unlock();
        m2.unlock();
    }
}

void task_02() {
    long long t2_counter = 0;
    while (true) {
        lock(m2, m1);
        //m2.lock();
        //m1.lock();
        cout << "task 2 : "<<t2_counter++<<endl;
        m1.unlock();
        m2.unlock();
    }
}

int main() {
    thread t1(task_01);
    thread t2(task_02);
    t1.join();
    t2.join();
    return 0;
}
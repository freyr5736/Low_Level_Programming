#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

int X = 0, Y = 0;
mutex m1, m2;

void wait_here(int seconds) {
    this_thread::sleep_for(chrono::seconds(seconds));
}

void increment_XY(int &XorY, mutex &m, const char *desc) {
    for (int i = 0; i < 5; ++i) {
        m.lock();
        ++XorY;
        cout << desc << XorY << endl;
        m.unlock();
        wait_here(1);
    }
}

void consume_XY() {
    int use_count = 5;
    int XplusY = 0;
    while (true) {
        int lock_result = try_lock(m1, m2);
        if (lock_result == -1) {
            if (X != 0 && Y != 0) {
                use_count--;
                XplusY += X + Y;
                X = 0;
                Y = 0;
                cout << "XplusY = " << XplusY << endl;
            }
            m1.unlock();
            m2.unlock();
            if (use_count == 0)
                break;
        } 
    }
}

int main() {
    thread t1(increment_XY, ref(X), ref(m1), "X = ");
    thread t2(increment_XY, ref(Y), ref(m2), "Y = ");
    thread t3(consume_XY);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
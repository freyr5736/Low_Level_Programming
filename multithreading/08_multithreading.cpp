#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

recursive_mutex m;
int buffer = 0;


void recur(char c, int loop_for) {
    if (!loop_for)
        return;

    m.lock();
    cout << "thread " << c << " locked " << endl;
    cout << "thread " << c << " : " << buffer++ << endl;
    recur(c, --loop_for);
    m.unlock();
    cout << "thread " << c << " unlocked " << endl;
}

int main() {
    thread t1(recur, '0', 5);
    thread t2(recur, '1', 5);
    t1.join();
    t2.join();
    return 0;
}

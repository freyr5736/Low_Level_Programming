#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

int count = 0;
int failed = 0;
timed_mutex m;

// try_lock_for
void for_increment() {
    for (int i = 0; i < 5; ++i) {
        if (m.try_lock_for(chrono::seconds(2))) {
            ++count;
            this_thread::sleep_for(chrono::seconds(1));
            cout << "count=" << count << " entered by tid=" << this_thread::get_id()
                 << "\n";
            m.unlock();
        } else {
            cout << "i=" << i
                 << " couldn't enter by tid=" << this_thread::get_id() << "\n";
            ++failed;
        }
    }
}

// try_lock_until
void until_increment() {
    for (int i = 0; i < 5; ++i) {
        auto now = chrono::steady_clock::now();
        if (m.try_lock_until(now + chrono::seconds(2))) {
            ++count;
            this_thread::sleep_for(chrono::seconds(1));
            cout << "count=" << count << " entered by tid=" << this_thread::get_id()
                 << "\n";
            m.unlock();
        } else {
            cout << "i=" << i
                 << " couldn't enter by tid=" << this_thread::get_id() << "\n";
            ++failed;
        }
    }
}

int main() {
    thread t1(for_increment);
    thread t2(for_increment);

    cout << "t1 id=" << t1.get_id() << "\n";
    cout << "t2 id=" << t2.get_id() << "\n";
    
    count = 0;
    failed = 0;

    t1.join();
    t2.join();
    cout << endl << "Using try_lock_for \n";
    cout << "counter incremented by " << count << "\n";
    cout << "entry failed " << failed << " times\n" << endl;

    count = 0;
    failed = 0;

    t1 = thread(until_increment);
    t2 = thread(until_increment);
    
    t1.join();
    t2.join();
    cout <<endl<< "Using try_lock_until \n";
    cout << "counter incremented by " << count << "\n";
    cout << "entry failed " << failed << " times" << endl;

    return 0;
}

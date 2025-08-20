#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono;

using ll = unsigned long long;

void sum_even(ll start, ll end, ll &out) {
    ll local = 0;
    for (ll i = start; i <= end; i += 2) {
        local += i;
    }
    out = local;
}

void sum_odd(ll start, ll end, ll &out) {
    ll local = 0;
    for (ll i = start; i <= end; i += 2) {
        local += i;
    }
    out = local;
}

int main() {
    ll start = 0;
    ll end = 1900000000;

    // without multithreading
    auto start_time = high_resolution_clock::now();
    cout << "Default (without multithreading)\n";

    ll even_local = 0;
    ll odd_local = 0;

    sum_even(start, end, even_local);
    sum_even(start, end, odd_local);
    auto stop_time = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(stop_time - start_time);

    cout << "even sum = " << even_local << "\n";
    cout << "odd  sum = " << odd_local << "\n";
    cout << "runtime  = " << dur.count() / 1000000.0 << " seconds\n"<<endl;


    // with multithreading
    start_time = high_resolution_clock::now();
    cout << "with multithreading\n";

    even_local = 0;
    odd_local = 0;

    thread t1(sum_even, start, end, std::ref(even_local));
    thread t2(sum_odd, start, end, std::ref(odd_local));

    t1.join();
    t2.join();

    stop_time = high_resolution_clock::now();
    dur = duration_cast<microseconds>(stop_time - start_time);

    cout << "even sum = " << even_local << "\n";
    cout << "odd  sum = " << odd_local << "\n";
    cout << "runtime  = " << dur.count() / 1000000.0 << " seconds\n";
}

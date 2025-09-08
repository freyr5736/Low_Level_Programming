#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <future>

using namespace std;
using namespace chrono;
using ull = unsigned long long;

void find_odd(promise<ull>&& odd_sum_promise, ull start, ull end){
    ull odd_sum = 0;
    for(ull i = start;i<=end;++i){
        if(i&1){
            odd_sum += i;
        }
    }
    odd_sum_promise.set_value(odd_sum);
}

int main(){
    ull start  =0, end = 1900000000;
    promise<ull> odd_sum;
    future<ull> odd_future = odd_sum.get_future();

    cout<<"Thread created successfully!"<<endl;
    thread t1 (find_odd,move(odd_sum),start,end);

    cout<<"waiting..."<<endl;
    cout<<"Odd Sum = "<<odd_future.get()<<endl;
    cout<<"task completed successfully!"<<endl;
    
    t1.join();
    return 0;

}
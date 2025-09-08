#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <future>

using namespace std;
using namespace chrono;
using ull = unsigned long long;

ull find_odd(ull start, ull end){
    cout<<"Function ThreadID : "<<this_thread::get_id()<<endl;
    ull odd_sum = 0;
    for(ull i = start;i<=end;++i){
        if(i&1){
            odd_sum += i;
        }
    }
    cout<<"Odd Sum = ";
    return odd_sum;
}

int main(){
    ull start  =0, end = 1900000000;
    cout<<"Main ThreadID : "<<this_thread::get_id()<<endl;
    //thread created if policy is launch::async
    future<ull> odd_sum = async(launch::deferred, find_odd, start, end);
    //future<ull> odd_sum = async(launch::async, find_odd, start, end);

    cout<<"wating..."<<endl;
    cout<<odd_sum.get()<<endl;
    cout<<"Completed!"<<endl;

    return 0;

}
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>

using namespace std;

mutex m;
condition_variable cv;
deque<int> buffer;
const unsigned int max_buffer_size = 50;

void producer(int val){
    while(val){
        unique_lock<mutex> locker(m);
        cv.wait(locker,[](){return buffer.size()<max_buffer_size;});
        buffer.push_back(val);
        cout<<val<<" produced"<<endl;
        val--;
        locker.unlock();
        cv.notify_one();
    }
}

void consumer(){
    while(true){
        unique_lock<mutex> locker (m);
        cv.wait(locker,[](){return buffer.size()>0;});
        int val = buffer.back();
        buffer.pop_back();
        cout<<val<<" consumed"<<endl;
        locker.unlock();
        cv.notify_one();
    }
}

int main(){
    thread t1,t2;
    t1 = thread(producer,100);
    t2 = thread(consumer);
    t1.join();
    t2.join();

    return 0;
}
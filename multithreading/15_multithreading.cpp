#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>

using namespace std;

mutex m;
condition_variable cv;
deque<int> buffer;
atomic<bool> done{false};
const unsigned int max_buffer_size = 50;

void producer(int val){
    while(val){
        unique_lock<mutex> locker(m);
        cv.wait(locker,[](){return buffer.size()<max_buffer_size;});
        buffer.push_back(val);
        cout<<val<<" produced"<<endl;
        val--;
        locker.unlock();
        cv.notify_all();
    }
    done = true;
}

void consumer(){
    while(true){
        unique_lock<mutex> locker (m);
        cv.wait(locker,[](){return buffer.size()>0;});
        int val = buffer.back();
        buffer.pop_back();
        cout<<val<<" consumed"<<endl;
        locker.unlock();
        cv.notify_all();
        if(done){
            break;
        }
    }
    std::cout<<"The End."<<endl;
}

int main(){
    thread t1,t2,t3;
    t1 = thread(producer,100);
    t2 = thread(consumer);
    t3 = thread(consumer);
    t1.join();
    t2.join(); 
    t3.join();

    return 0;
}
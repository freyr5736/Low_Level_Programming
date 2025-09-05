#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;

condition_variable cv;
mutex m;
long bal = 0;

void add_moni(int moni){
    lock_guard<mutex> lg(m);
    bal+=moni;
    cout<<"curr bal = "<<bal<<endl;
    //cv.notify_all(); // notfies all the sleeping threads that the lock is free
    cv.notify_one(); // notifies one of the sleeping threads that the lock is free
}

void withdraw_moni(int moni){
    unique_lock<mutex> ul(m);
    cv.wait(ul,[]{return (bal!=0)?true : false;}); // if returns false, the thread will wait until the condition is true
    if(bal>moni){
        bal-=moni;
        cout<<"withdraw amount = "<<moni<<endl;
    }
    else{
        cout<<"insufficient balance"<<endl;
    }
    cout<<"curr bal = "<<bal<<endl;
}

int main(){
    thread t1(withdraw_moni,500);
    this_thread::sleep_for(chrono::seconds(2));
    thread t2(add_moni, 2000);
    t1.join();
    t2.join();
    return 0;
}
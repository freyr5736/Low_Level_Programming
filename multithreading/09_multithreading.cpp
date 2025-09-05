#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int buffer = 0;
mutex m;

void task(int loop_for){
    lock_guard<mutex> lock(m);  // light weight wrapper for owning the mutex on scoped basis
    for(int i=0;i<loop_for;++i){
        buffer++;
        cout<<"thread "<<this_thread::get_id()<<" : "<<buffer<<endl;
    }
        cout<<endl;
}// after existing the scope, lock_gaurd destructor will unlock themutex lock

int main(){
    thread t1;
    thread t2;
    t1 = thread(task,5);
    t2 = thread(task,5);

    t1.join();
    t2.join();

    return 0;

}
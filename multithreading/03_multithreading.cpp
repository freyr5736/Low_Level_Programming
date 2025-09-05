#include <iostream>
#include <chrono>
#include <thread>

using namespace std;


// JOIN() working
void run(int count){
    while(count--)
        cout<<"print "<<count<<endl;
    this_thread::sleep_for(chrono::seconds(3));
}


int main(){

    thread t1(run,11);
    cout<<"Executed before .join()\n";

    //t1.join();

    //t1.join(); // double join() terminates the program. (this will compile) (runtime error std::syste_error)
    
    // if(t1.joinable()) // checking if thread is joinable. This will run as expected and throw no errors.
    //     t1.join();

    // detach separates newly created thread from parent thread
    t1.detach();   // thread wont wait for 3 seconds, will terminate right away. If parent thread terminates before child thread then the whole program terminates
    //t1.detach(); // double detach() terminates the program. (this will compile) (runtime error std::syste_error)
    // if(t1.joinable()) // checking if thread is joinable. This will run as expected and throw no errors.
    //     t1.detach();

    cout<<"Executed after .join()\n"; // this will execute later. (After all threads join together n output run())

    return 0;
}
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;


// JOIN() working
void run(int count){
    while(count--)
        cout<<"print "<<count<<endl;
    

    std::this_thread::sleep_for(chrono::seconds(3));
}


int main(){

    std::thread t1(run,11);
    cout<<"Executed before .join()\n";

    t1.join();

    //t1.join(); // double join() terminates the program. (this will compile) (runtime error std::syste_error)
    
    if(t1.joinable()) // checking if thread is joinable. This will run as expected and throw no errors.
        t1.join();

    cout<<"Executed after .join()\n"; // this will execute later. (After all threads join together n output run())

    return 0;
}
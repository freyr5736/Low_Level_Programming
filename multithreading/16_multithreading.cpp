#include <iostream>
#include <thread>
#include <deque>
#include <vector>
#include <atomic>
#include <semaphore.h>
using namespace std;

constexpr unsigned int max_buffer_size = 50;

deque<int> buffer_q;
sem_t sem_empty; // counts free slots
sem_t sem_full;  // counts available items
sem_t sem_crit;  // binary semaphore for critical section (mutual exclusion)
atomic<bool> done{false};
int consumers_total = 2; // adjust to number of consumer threads

void producer(int start_val) {
    for (int v = start_val; v >= 1; --v) {
        sem_wait(&sem_empty);       // wait for a free slot
        sem_wait(&sem_crit);        // enter critical section
        buffer_q.push_back(v);
        cout << v << " produced" << endl;
        sem_post(&sem_crit);        // exit critical section
        sem_post(&sem_full);        // signal one available item
    }

    // Termination via poison pills (sentinels) so consumers can stop
    // A deliberately crafted “shutdown” message pushed into the queue; when a consumer dequeues it
    // Push one sentinel per consumer.
    for (int i = 0; i < consumers_total; ++i) {
        sem_wait(&sem_empty);
        sem_wait(&sem_crit);
        buffer_q.push_back(0);      // 0 is sentinel
        sem_post(&sem_crit);
        sem_post(&sem_full);
    }
    done.store(true, memory_order_release);
}

void consumer() {
    while (true) {
        sem_wait(&sem_full);        // wait for an item
        sem_wait(&sem_crit);        // enter critical section
        int val = buffer_q.back();
        buffer_q.pop_back();
        sem_post(&sem_crit);        // exit critical section
        sem_post(&sem_empty);       // free a slot

        if (val == 0) {             // sentinel => terminate this consumer
            cout << "The End." << endl;
            break;
        }
        cout << val << " consumed" << endl;
    }
}

int main() {
    // Initialize semaphores
    sem_init(&sem_empty, 0, max_buffer_size); // all slots empty at start [N]
    sem_init(&sem_full,  0, 0);               // no items initially
    sem_init(&sem_crit,  0, 1);               // binary semaphore for CS

    thread t1(producer, 100);
    thread t2(consumer);
    thread t3(consumer);

    t1.join();
    t2.join();
    t3.join();

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    sem_destroy(&sem_crit);
    return 0;
}

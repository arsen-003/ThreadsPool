#include <iostream>
#include "parallel_scheduler.cpp"

void print_task(void* arg) {
    int* val = static_cast<int*>(arg);
    std::cout << "Task executed with value: " << *val << std::endl;
}

int main() {
    parallel_scheduler scheduler(3); // Pool with 3 threads

    int args[10];
    for (int i = 0; i < 10; ++i) {
        args[i] = i;
        scheduler.run(print_task, &args[i]);
    }

    return 0; // Destructor will clean up
}

#include <iostream>
#include "process.hpp"
#include <queue>

int main(int argc, char* argv[]) {

    //prompt for queues and tq

    //set up queues
    std::priority_queue<Process(), std::vector<Process()>, std::greater<Process()> > readyQueue;
    Process* p = new Process(2,3,4,5,6,7);
        readyQueue.push(p);
    //read in file and create processes

    //do the scheduler


    return 0;
}


#ifndef PROCESS_HPP
#define PROCESS_HPP

class Process{
    public:
        int pid;
        int burst;
        int arr;
        int pri;
        int deadline;
        int io;

    Process();
    Process(int pid, int bst, int arr, int pri, int ddline, int io);
    ~Process();


};



#endif

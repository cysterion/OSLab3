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
        int age;
	
		void print();

    Process();
    Process(int pid, int bst, int arr, int pri, int ddline, int io);
    ~Process();

		friend bool operator<(const Process& a, const Process& b) {
			return a.deadline > b.deadline;
		}
		friend bool operator>(const Process& a, const Process& b) {
			return b.arr < a.arr;
		}

};



#endif

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
        int tickArrived;
        int start;
        int currentPri;
				int ioFinish;
	int initialBurst;
	
		void print();

    Process();
    Process(int pid, int bst, int arr, int pri, int ddline, int io);
    ~Process();

		class IOCompare {
		public:
			bool operator()(const Process &lhs, const Process &rhs) const;
		};
	
		class RTSCompare {
		public:
			bool operator()(const Process &a, const Process &b) const;
		};
	
		class ArrCompare {
		public:
			bool operator()(const Process &a, const Process &b) const;
		};
};




#endif

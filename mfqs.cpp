#include <iostream>
#include "process.hpp"
#include <queue>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {

    //prompt for queues and tq
    cout << "Enter number of queues (max 5)\n";

    int numQ;
    cin >> numQ;
    cout << "Enter Queue 1 Time Quantum\n";
    int tq;
    cin >> tq;
    //set up queues
	

	priority_queue<Process, vector<Process>, greater<Process>> readyQueue;
	
	deque<Process> queues [numQ];
	for(int i = 0; i < numQ; i++){
		deque<Process> tempQ;
		queues[i] = tempQ;
	}
	
	//read in file and create processes
	ifstream file;
//	cout << argv[1];
	file.open (argv[1]);
	string line;

	if (file.is_open()) {
		Process* p = new Process();

		while (file >> p->pid >> p->arr >> p->burst >> p->pri >> p->deadline >> p->io) {
//			p->print();
			readyQueue.push(*p);
		}
		file.close();
	}

	//do the scheduler
	unsigned int tick = 0;
	int timeQuantum = 0;
	int runtime = 0;
	int sourceQ = 0;
	bool running = false;
	Process runningProcess;
	
	while(!readyQueue.empty()) {
		
		Process p = readyQueue.top();
		while (p.arr == tick){
			readyQueue.pop();
			queues[0].push_back(p);
			
			p = readyQueue.top();
		}
		
		if(running) {
			runningProcess.burst -= 1;
			
			if(runtime == timeQuantum) {
				queues[sourceQ+1].push_back(runningProcess);
				running = false;
			}
			
		} else {
			int i = 0;
			deque<Process> currQ = queues[i];
			while(currQ.empty()) {
				currQ = queues[i++];
			}
			
			runningProcess = queues[i].front();
			queues[i].pop_front();
			
			sourceQ = 0;
			timeQuantum = tq;
			running = true;
			
			runtime++;
			runningProcess.burst -= 1;
		}
	
		
		
		
		tick++;
	}
	
	return 0;
}


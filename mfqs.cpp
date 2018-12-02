#include <iostream>
#include "process.hpp"
#include <queue>
#include <fstream>
#include <math.h>

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

		while (file >> p->pid >> p->burst >> p->arr >> p->pri >> p->deadline >> p->io) {
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
	bool processessLeft = true;
	
	while(!readyQueue.empty() || processessLeft) {
		
		Process p = readyQueue.top();
		
		while (p.arr == tick && !readyQueue.empty()){
			cout << "Added: " << p.pid << " to Q: 0" << endl;
			
			queues[0].push_back(p);
			readyQueue.pop();
			
			if (!readyQueue.empty()) {
				p = readyQueue.top();
			}
		}
		
		if(running) {
			runningProcess.burst -= 1;
			runtime++;
			cout << "Currently Running: " << runningProcess.pid << endl;
			
			if(runningProcess.burst == 0) {
				running = false;
				runtime = 0;
				cout << "Process: " << runningProcess.pid << " finished" << endl;
				
			} else {
				if((runtime >= timeQuantum) && (sourceQ < (numQ-1))) {
					cout << "Demoted: " << runningProcess.pid << " to Q: " << sourceQ+1 << endl;
					queues[sourceQ+1].push_back(runningProcess);
					runtime = 0;
					running = false;
				}
				
				if ((runtime >= timeQuantum) && (sourceQ == (numQ-1))) {
					queues[numQ-1].push_back(runningProcess);
					runtime = 0;
					running = false;
				}
			}
			
		} else {
			int i = 0;
			deque<Process> currQ = queues[i];
			while(currQ.empty() && i < numQ) {
				currQ = queues[++i];
			}
			
			if (i < numQ) {
				runningProcess = queues[i].front();
				queues[i].pop_front();
				
				sourceQ = i;
				if(sourceQ == (numQ-1)) {
					timeQuantum = 1;
				} else {
					timeQuantum = tq * pow(2, i);
				}
				running = true;
				
//				runtime++;
//				runningProcess.burst -= 1;
//				cout << "Currently Running: " << runningProcess.pid << endl;
			}
		}
	
//		if (running) {
//			cout << "Currently Running: " << runningProcess.pid << endl;
//		}
//		cout << tick << endl;
		tick++;
		if (readyQueue.empty()) {
			
			for(int i = 0; i < numQ; i++) {
				if (!queues[i].empty()) {
					processessLeft = true;
					break;
					
				} else {
					if (runningProcess.burst == 0) {
						processessLeft = false;
					}
				}
			}
			
		}
	}
	
	return 0;
}


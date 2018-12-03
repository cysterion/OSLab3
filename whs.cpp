#include <iostream>
#include "process.hpp"
#include <queue>
#include <fstream>
#include <math.h>
#include <deque>
#include <cstring>
#include <cctype>

using namespace std;

int main(int argc, char* argv[]) {
	
	cout << "Enter Queue 1 Time Quantum\n";
	int tq;
	cin >> tq;
	
	cout << "Enter Aging interval\n";
	int aInt;
	cin >> aInt;
	
	//set up queues
	int numQ = 100;
	
	
	priority_queue<Process, vector<Process>, greater<Process>> readyQueue;
	
	deque<Process> queues [numQ];
//	deque<Process> lowBandQs [50];
	
	for(int i = 0; i < numQ; i++){
		deque<Process> tempQ;
		queues[i] = tempQ;
	}
	
	//read in file and create processes
	ifstream file;
	//	cout << argv[1];
	file.open (argv[1]);
	string line;
	string parts[6];
	
	if (file.is_open()) {
		Process* p = new Process();
		//		while (file >> p->pid >> p->burst >> p->arr >> p->pri >> p->deadline >> p->io) {
		while (getline(file, line, '\n')) {
			bool valid = true;
			//			cout << line.c_str() << '\n';
			
			// split line into str[]
			int i = 0;
			char* l = strdup(line.c_str());
			char *token = std::strtok(l, "\t");
			
			while (token != NULL) {
				//				std::cout << token << '\n';
				parts[i] = token;
				i++;
				token = std::strtok(NULL, "\t");
			}
			
			//check if any elmns are !numerical
			int ints[6];
			for (int i = 0; i < 6; i++) {
				try {
					int t = stoi(parts[i]);
					ints[i] = t;
					
				} catch (const std::invalid_argument& ia) {
#ifdef DEBUG
					std::cerr << "Invalid argument: " << ia.what() << '\n';
#endif
					valid = false;
				}
			}
			
			//todo:check for things that might break the scheduler
			if (ints[0] < 0
					|| ints[1] < 0
					|| ints[2] < 0
					|| ints[3] < 0
					|| ints[5] < 0){
				valid = false;
			}
			
			if(valid) {
				//create the process
				p->pid = ints[0];
				p->burst = ints[1];
				p->arr = ints[2];
				p->pri = ints[3];
				p->deadline = ints[4];
				p->io = ints[5];
				
				readyQueue.push(*p);
			} else {
#ifdef DEBUG
				cout << "Invalid line: " << line << endl;
#endif
			}
			
		}
		file.close();
	}
	
	
	int numProc = readyQueue.size();
	unsigned int waitTime = 0;
	unsigned int turnTime = 0;
	
	//do the scheduler
	unsigned int tick = 0;
	int timeQuantum = 0;
	int runtime = 0;
	int sourceQ = 0;
	bool running = false;
	Process runningProcess;
//	bool processessLeft = true;
	
	while(!readyQueue.empty()) {
		
		Process p = readyQueue.top();
		
		while (p.arr == tick && !readyQueue.empty()){
#ifdef DEBUG
			cout << "Added: " << p.pid << " to Q" << p.pri << endl;
#endif
			queues[p.pri].push_back(p);
			readyQueue.pop();
			
			if (!readyQueue.empty()) {
				p = readyQueue.top();
			}
		}
		
		if (running) {
			runningProcess.burst -= 1;
			runtime++;
#ifdef DEBUG
			cout << "Process " << runningProcess.pid << " is running" << endl;
#endif
			
			if(runningProcess.burst == 0) {
				running = false;
				runtime = 0;
#ifdef DEBUG
				cout << "Process: " << runningProcess.pid << " finished" << endl;
#endif
				
				turnTime += tick - runningProcess.arr;
			} else {
				
				
				if((runtime >= timeQuantum)) {
#ifdef DEBUG
					cout << "Demoted: " << runningProcess.pid << " to Q: " << sourceQ+1 << endl;
#endif
					if ((sourceQ <= 49)) {
						if ((sourceQ+tq) <= 49) {
							queues[sourceQ+tq].push_back(runningProcess);
							queues[sourceQ].pop_front();
							running = false;
							runtime = 0;
							
						} else {
							queues[49].push_back(runningProcess);
							queues[sourceQ].pop_front();
							running = false;
							runtime = 0;
						}
						
					} else {
						if ((sourceQ+tq) <= 99) {
							queues[sourceQ+tq].push_back(runningProcess);
							queues[sourceQ].pop_front();
							running = false;
							runtime = 0;
							
						} else {
							queues[99].push_back(runningProcess);
							queues[sourceQ].pop_front();
							running = false;
							runtime = 0;
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
				
//				if (i == 0){
//					waitTime += tick - runningProcess.arr;
//				}
//
				sourceQ = i;
//				if(sourceQ == (numQ-1)) {
//					timeQuantum = 1;
//				} else {
//					timeQuantum = tq * pow(2, i);
//				}
				timeQuantum = tq;
				running = true;
			}
		}
		
		tick++;
	}
	waitTime = waitTime / numProc;
	turnTime = turnTime / numProc;
	cout << "Average Wait Time is: " << waitTime << endl;
	cout << "Average Turnaround Time is: " << turnTime << endl;
	cout << "Total Processes: " << numProc << endl;
	return 0;
}

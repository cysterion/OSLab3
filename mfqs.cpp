#include <iostream>
#include "process.hpp"
#include <queue>
#include <fstream>
#include <math.h>
#include <deque>

using namespace std;

int main(int argc, char* argv[]) {

    //prompt for queues and tq
    cout << "Enter number of queues (max 5)\n";

    int numQ;
    cin >> numQ;
    cout << "Enter Queue 1 Time Quantum\n";
    int tq;
    cin >> tq;

	cout << "Enter Aging interval\n";
	int aInt;
	cin >> aInt;
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
				std::cerr << "Invalid argument: " << ia.what() << '\n';
				valid = false;
			}
		}
		
		//todo:check for things that might break the scheduler
		if (ints[0] < 0
				|| ints[1] < 0
				|| ints[2] < 0
				|| ints[3] < 0){
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
			cout << "Invalid line: " << line << endl;
		}
		
	}
	file.close();
}

    int numProc = readyQueue.size();
    int waitTime = 0;
    int turnTime = 0;

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
				turnTime += tick - runningProcess.arr;
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
			    if (i == 0){
                    waitTime += tick - runningProcess.arr;
                }
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

//		deque<Process> aq = queues[numQ-1];
//		for (int i = 0; i < aq.size(); i++) {
//			aq[i].age += 1;

//			if (aq[i].age >= aInt) {
//				cout << "Aged up: " << aq[i].pid << endl;
//				aq[i].age = 0;
//				queues[numQ-2].push_back(aq[i]);
//				queues[numQ-1].erase(aq.begin()+i);
//			}

//		}

//		for (auto it = queues[numQ-1].begin(); it != queues[numQ-1].end(); ++it) {
//			cout << *it;
//			it->age += 1;j
//			if (it->age >= aInt) {
//				cout << "Aged up: " << it->pid << endl;
////				it->age = 0;
//				queues[numQ-2].push_back(*it);
//				queues[numQ-1].erase(it);
//			}
//		}


		tick++;
	}
    waitTime = waitTime / numProc;
    turnTime = turnTime / numProc;
	cout << "Average Wait Time is: " << waitTime << endl;
    cout << "Average Turnaround Time is: " << turnTime << endl;
    cout << "Total Processes: " << numProc << endl;
	return 0;
}


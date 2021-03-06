#include <iostream>
#include "process.hpp"
#include <queue>
#include <fstream>
#include <math.h>
//#include <deque>
#include <cstring>
#include <cctype>
#include <stdexcept>

using namespace std;

int main(int argc, char* argv[]) {
	
//	int numQ = stoi(argv[2]);
//	int tq = stoi(argv[3]);
//	int aInt = stoi(argv[4]);

    //prompt for queues and tq
    cout << "Enter number of queues (max 5)\n";

    int numQ;
    cin >> numQ;
	if (numQ <= 1 || numQ > 5) {
		exit(-1234);
	}

    cout << "Enter Queue 1 Time Quantum\n";
    int tq;
    cin >> tq;

	cout << "Enter Aging interval\n";
	int aInt;
	cin >> aInt;
    //set up queues


	priority_queue<Process, vector<Process>, Process::ArrCompare> readyQueue;

	queue<Process> queues [numQ];
	for(int i = 0; i < numQ; i++){
		queue<Process> tempQ;
		queues[i] = tempQ;
	}

	//read in file and create processes
	ifstream file;
//	cout << argv[1];
	file.open (argv[1]);
	string line;
	string parts[6];

	if (file.is_open()) {
		Process p;
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
					|| ints[3] < 0){
				valid = false;
			}
			
			if(valid) {
				//create the process
				p.pid = ints[0];
				p.burst = ints[1];
				p.arr = ints[2];
				p.pri = ints[3];
				p.deadline = ints[4];
				p.io = ints[5];
				
				readyQueue.push(p);
			} else {
#ifdef DEBUG
				cout << "Invalid line: " << line << endl;
#endif
			}
			
		}
		file.close();
	}

	int numProc = readyQueue.size();
	unsigned long long waitTime = 0, turnTime = 0;

	//do the scheduler
	unsigned long long tick = 0;
	int timeQuantum = 0;
	int runtime = 0;
	int sourceQ = 0;
	bool running = false;
	Process runningProcess;
	bool processessLeft = true;

	while(!readyQueue.empty() || processessLeft) {
#ifdef DEBUG
		cout << "Tick: " << tick << endl;
#endif

		Process p = readyQueue.top();

		while (p.arr == tick && !readyQueue.empty()){
#ifdef DEBUG
			cout << "Added: " << p.pid << " to Q: 0" << endl;
#endif

			queues[0].push(p);
			readyQueue.pop();

			if (!readyQueue.empty()) {
				p = readyQueue.top();
			}
		}

		if(running) {
			runningProcess.burst -= 1;
			runtime++;
#ifdef DEBUG
			cout << "Currently Running: " << runningProcess.pid << endl;
#endif

			if(runningProcess.burst == 0) {
				running = false;
				runtime = 0;
#ifdef DEBUG
				cout << "Process: " << runningProcess.pid << " finished" << endl;
#endif
				waitTime += tick - runningProcess.arr - runningProcess.initialBurst;
				turnTime += tick - runningProcess.arr;
			} else {
				if((runtime >= timeQuantum) && (sourceQ < (numQ-1))) {
#ifdef DEBUG
					cout << "Demoted: " << runningProcess.pid << " to Q: " << sourceQ+1 << endl;
#endif
					if((sourceQ+1) == (numQ -1)) {
						runningProcess.tickArrived = tick;
					}
					queues[sourceQ+1].push(runningProcess);
					runtime = 0;
					running = false;
				}

				if ((runtime >= timeQuantum) && (sourceQ == (numQ-1))) {
					queues[numQ-1].push(runningProcess);
					runtime = 0;
					running = false;
				}
			}

		} else {
			int i = 0;
			queue<Process> currQ = queues[i];
			while(currQ.empty() && i < numQ) {
				currQ = queues[++i];
			}

			if (i < numQ) {
				runningProcess = queues[i].front();
				queues[i].pop();
				sourceQ = i;
				if(sourceQ == (numQ-1)) {
					timeQuantum = 1;
				} else {
					timeQuantum = tq * pow(2, i);
				}
				running = true;
			}
		}
		
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
		
		if (!queues[numQ-1].empty()) {
			Process ap = queues[numQ-1].front();
			if (ap.tickArrived == (tick-aInt)) {
				queues[numQ-2].push(ap);
				queues[numQ-1].pop();
#ifdef DEBUG
				cout << "Aged up: " << ap.pid << " to Q: " << numQ-2 << endl;
#endif
			}
		}
#ifdef DEBUG
		cout << tick <<endl;
#endif
		tick++;
	}
//    waitTime = waitTime / numProc;
//    turnTime = turnTime / numProc;
	cout << "Average Turn Time: " << ((float)turnTime) / numProc << endl;
	cout << "Average Wait Time: " << ((float)waitTime) / numProc << endl;
    cout << "Total Processes: " << numProc << endl;
	return 0;
}


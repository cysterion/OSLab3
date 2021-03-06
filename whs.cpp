#include <iostream>
#include "process.hpp"
#include <queue>
#include <fstream>
#include <math.h>
#include <deque>
#include <cstring>
#include <cctype>
#include <stdexcept>

using namespace std;

int main(int argc, char* argv[]) {

//	int tq=stoi(argv[3]);
//	int aInt = stoi(argv[4]);

	cout << "Enter Queue 1 Time Quantum\n";
	int tq;
	cin >> tq;

	cout << "Enter Aging interval\n";
	int aInt;
	cin >> aInt;

	//set up queues
	int numQ = 100;


	priority_queue<Process, vector<Process>, Process::ArrCompare> readyQueue;

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
	unsigned long long waitTime = 0;
	unsigned long long turnTime = 0;

	//do the scheduler

	priority_queue <Process, vector<Process>, Process::IOCompare> IOQueue;
	unsigned long long tick = 0;
	int timeQuantum = 0;
	int runtime = 0;
	int sourceQ = 0;
	bool running = false;
	Process runningProcess;
	bool processessLeft = true;

	while(!readyQueue.empty() || processessLeft) {

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
				waitTime += tick - runningProcess.arr - runningProcess.initialBurst - runningProcess.io;
			} else {

				if (runtime == timeQuantum-1 && (runningProcess.io != 0)) {
					runningProcess.currentPri = runningProcess.pri - runningProcess.io;

					if (runningProcess.currentPri <= 0) {
						runningProcess.currentPri = 0;
					} else {
						if (runningProcess.pri >= 50 && runningProcess.currentPri <= 49) {
							runningProcess.currentPri = 50;
						}
					}

					runningProcess.ioFinish = tick + runningProcess.io;

					IOQueue.push(runningProcess);
					runtime = 0;
#ifdef DEBUG
					cout << "Sent: " << runningProcess.pid << " to IO" << endl;
#endif
				}

				if((runtime >= timeQuantum)) {
					if ((sourceQ+tq) > runningProcess.pri) {
						runningProcess.tickArrived = 0;
						runningProcess.currentPri = runningProcess.pri;
						queues[runningProcess.pri].push_back(runningProcess);
						running = false;
						runtime = 0;
#ifdef DEBUG
						cout << "Demoted: " << runningProcess.pid << " to Q: " << runningProcess.pri << endl;
#endif
					} else {
						if ((sourceQ <= 49)) {
							if ((sourceQ+tq) <= 49) {
								runningProcess.tickArrived = 0;
								runningProcess.currentPri = sourceQ+tq;
								queues[sourceQ+tq].push_back(runningProcess);
								running = false;
								runtime = 0;
		#ifdef DEBUG
								cout << "Demoted: " << runningProcess.pid << " to Q: " << sourceQ+tq << endl;
		#endif
							} else {
								runningProcess.tickArrived = 0;
								runningProcess.currentPri = 49;
								queues[49].push_back(runningProcess);
								running = false;
								runtime = 0;
		#ifdef DEBUG
								cout << "Demoted: " << runningProcess.pid << " to Q: " << 49 << endl;
		#endif
							}

						} else {
							if ((sourceQ+tq) <= 99) {
								runningProcess.tickArrived = 0;
								runningProcess.currentPri = sourceQ+tq;
								queues[sourceQ+tq].push_back(runningProcess);
								running = false;
								runtime = 0;
		#ifdef DEBUG
								cout << "Demoted: " << runningProcess.pid << " to Q: " << sourceQ+tq << endl;
		#endif
							} else {
								runningProcess.tickArrived = 0;
								runningProcess.currentPri = 99;
								queues[99].push_back(runningProcess);
								running = false;
								runtime = 0;
		#ifdef DEBUG
								cout << "Demoted: " << runningProcess.pid << " to Q: " << 99 << endl;
		#endif
							}
						}
					}
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

				if (runningProcess.start == -1) {
					runningProcess.start = tick;
				}
			}
		}


			while(!IOQueue.empty() && IOQueue.top().ioFinish == tick){
				Process temp = IOQueue.top();
				temp.io = 0;
				queues[temp.currentPri].push_back(temp);
				IOQueue.pop();

#ifdef DEBUG
				cout << "Sent: " << temp.pid << " from IO to Q: " << temp.currentPri << endl;
#endif


		}

		for (int i = 90; i < 100; i++) {
			deque<Process> q = queues[i];
				if (!q.empty()) {
					Process ap = queues[i].front();

					if (ap.tickArrived == (tick-aInt)) {
						queues[ap.currentPri-10].push_back(ap);
						q.pop_front();
#ifdef DEBUG
						cout << "Aged up: " << ap.pid << " to Q: " << numQ-2 << endl;
#endif
					}
				}
		}

		if (readyQueue.empty()) {

			for(int i = 0; i < numQ; i++) {
				if (!queues[i].empty()) {
					processessLeft = true;
					break;

				} else if(!IOQueue.empty()){
                    processessLeft = true;
                    break;
                } else{
					if (runningProcess.burst == 0) {
						processessLeft = false;
					}
				}
			}
		}
#ifdef DEBUG
		cout << tick <<endl;
#endif
		tick++;
	}
//	waitTime = waitTime / numProc;
//	turnTime = turnTime / numProc;
	cout << "Average Turn Time: " << ((float)turnTime) / numProc << endl;
	cout << "Average Wait Time: " << ((float)waitTime) / numProc << endl;
	cout << "Total Processes: " << numProc << endl;
	return 0;
}

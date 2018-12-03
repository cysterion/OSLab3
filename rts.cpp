#include <iostream>
#include "process.hpp"
#include <queue>
#include <fstream>
#include <math.h>
#include <deque>
#include <cstring>
#include <cctype>

using namespace std;

void deathByHardMode(unsigned int turnTime, unsigned int waitTime, int numProc, int succeedProc) {
	turnTime = turnTime / succeedProc;
	waitTime = waitTime / numProc;
	cout << "Number of Processes: " << numProc << endl;
	cout << "Succeeded Proccesses: " << succeedProc << endl;
	cout << "Average Turn Time: " << turnTime << endl;
	cout << "Average Wait Time: " << waitTime << endl;
	exit(-32);
}

int main(int argc, char* argv[]) {
	
	
	cout << "Hard (0) or Soft (1)\n";
	int type;
	cin >> type;
	
	priority_queue<Process, vector<Process>, greater<Process>> readyQueue;
	
	priority_queue<Process, vector<Process>, less<Process>> queue;
	
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
					|| ints[4] < 0){
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
	
	//do the scheduler
	unsigned int tick = 0;
	Process runningProcess;
	unsigned int turnTime = 0;
	unsigned int waitTime = 0;
	int succeedProc = 0;
	int numProc = readyQueue.size();
	
	while(!readyQueue.empty() || !queue.empty()) {
		
		Process p = readyQueue.top();
		while (p.arr == tick && !readyQueue.empty()){
#ifdef DEBUG
			cout << "Added: " << p.pid << " to Q" << endl;
#endif
			queue.push(p);
			readyQueue.pop();
			
			if (!readyQueue.empty()) {
				p = readyQueue.top();
			}
		}
		
		if (!queue.empty()) {
			runningProcess = queue.top();
			queue.pop();
			
			runningProcess.burst -= 1;
			if (runningProcess.start == -1) {
				runningProcess.start = tick;
			}
#ifdef DEBUG
			cout << "Process " << runningProcess.pid << " is running 🏃‍♂️" << endl;
#endif
			
			if (runningProcess.burst == 0) {
				turnTime += tick - runningProcess.arr;
				waitTime += runningProcess.start - runningProcess.arr;
				succeedProc += 1;
#ifdef DEBUG
				cout << "Process " << runningProcess.pid << " completed" << endl;
#endif
				
			} else {
				if (runningProcess.deadline <= tick) {
#ifdef DEBUG
					cout << "Process " << runningProcess.pid << " failed to meet deadline" << endl;
#endif
					runningProcess.print();
					cout << tick << endl;
					
					if(type == 0){
						deathByHardMode(turnTime, waitTime, numProc, succeedProc);
					}
					
				} else {
					queue.push(runningProcess);
				}
			}
		}
		
		int i = 0;
		bool end = false;
		while(!end) {
			Process temp = queue.top();
			queue.pop();
			
			if (!((temp.deadline - tick) < temp.burst)) {
				queue.push(temp);
				
			} else {
#ifdef DEBUG
				cout << "Process " << temp.pid << " failed to meet deadline" << endl;
#endif
				
				if(type == 0){
					deathByHardMode(turnTime, waitTime, numProc, succeedProc);
				}
			}
			
			if (i >= queue.size()) {
				end = true;
			}
			i++;
		}
		
#ifdef DEBUG
		cout << tick <<endl;
#endif
		tick++;
	}
	
	turnTime = turnTime / succeedProc;
	waitTime = waitTime / numProc;
	cout << "Number of Processes: " << numProc << endl;
	cout << "Succeeded Proccesses: " << succeedProc << endl;
	cout << "Average Turn Time: " << turnTime << endl;
	cout << "Average Wait Time: " << waitTime << endl;
	
	return 0;
}

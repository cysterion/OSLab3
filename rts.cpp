#include <iostream>
#include "process.hpp"
#include <queue>
#include <fstream>
#include <math.h>
#include <deque>

using namespace std;

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
	Process runningProcess;
	
	while(!readyQueue.empty() || !queue.empty()) {
		
		Process p = readyQueue.top();
		while (p.arr == tick && !readyQueue.empty()){
			cout << "Added: " << p.pid << " to Q" << endl;
			
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
			cout << "Process " << runningProcess.pid << " is running 🏃‍♂️" << endl;
			
			if (runningProcess.deadline <= tick) {
				cout << "Process " << runningProcess.pid << " failed to meet deadline" << endl;
				
				if(type == 0){
					exit(-1);
				}
				
			} else {
				if (runningProcess.burst == 0) {
					cout << "Process " << runningProcess.pid << " completed" << endl;
					
				} else {
					queue.push(runningProcess);
				}
			}
		}
		
		tick++;
	}
	
	return 0;
}

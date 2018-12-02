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
	

	std::priority_queue<Process, std::vector<Process>, std::greater<Process>> readyQueue;
	
	std::priority_queue<Process, std::vector<Process>, std::greater<Process>> queues [numQ];
	for(int i = 0; i < numQ; i++){
		std::priority_queue<Process, std::vector<Process>, std::greater<Process>> tempQ;
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
	
	while(!readyQueue.empty()) {
		Process p = readyQueue.top();
		while (p.arr == tick){
			readyQueue.pop();
			queues[0].push(p);
			
			p = readyQueue.top();
		}
	
		
		
		
		tick++;
	}
	
	return 0;
}


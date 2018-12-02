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
//    Process* p = new Process(1,2,3,4,5,6);
//		Process* p2 = new Process(2,3,2,5,6,7);
//    readyQueue.push(*p);

   std::priority_queue<Process, std::vector<Process>, std::greater<Process>> queues [numQ];
    for(int i = 0; i < numQ; i++){
        std::priority_queue<Process, std::vector<Process>, std::greater<Process>> tempQ;
        queues[i] = tempQ;
    }

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

	Process t = readyQueue.top();
	t.print();

    //read in file and create processes

    //do the scheduler


    return 0;
}


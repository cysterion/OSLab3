#include "process.hpp"
#include <iostream>

using namespace std;

//Pid	Bst	Arr	Pri Ddline	I/O

Process::Process(){}

Process::Process(int pid, int bst, int arr, int pri, int ddline, int io) {
	this->pid = pid;
	this->burst = bst;
	this->arr = arr;
	this->pri = pri;
	this->deadline = ddline;
	this->io = io;
	this->tickArrived = 0;
	this->start = -1;
	this->currentPri = pri;
	this->ioFinish = 0;
	this->initialBurst = bst;
}

Process::~Process() {
	
}

void Process::print() {
	cout << pid << ", " << burst << ", " << arr << ", " << pri << ", " << deadline << ", " << io << endl;
}

bool Process::IOCompare::operator()(const Process &lhs, const Process &rhs) const {

	return lhs.io > rhs.io;
}

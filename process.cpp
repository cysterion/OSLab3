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
}

Process::~Process() {
	
}

void Process::print() {
	cout << pid << ", " << burst << ", " << arr << ", " << pri << ", " << deadline << ", " << io << ", " << initialBurst << endl;
}

bool Process::IOCompare::operator()(const Process &lhs, const Process &rhs) const {
	return lhs.io > rhs.io;
}

bool Process::ArrCompare::operator()(const Process &a, const Process &b) const {
	return b.arr < a.arr;
}

bool Process::RTSCompare::operator()(const Process &a, const Process &b) const {
	// arr, dd, pri, pid
	if (b.deadline == a.deadline) {
		if (b.pri == a.pri) {
			return b.pid < a.pid;
		}
		return b.pri < a.pri;
	}
	return b.deadline < a.deadline;
}


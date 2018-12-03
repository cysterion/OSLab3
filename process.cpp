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
	this->age = 0;
}

Process::~Process() {
	
}

void Process::print() {
	cout << pid << ", " << burst << ", " << arr << ", " << pri << ", " << deadline << ", " << io << endl;
}

bool Process::validate() {
	if (this->pid < 0
			|| this->burst < 0
			|| this->arr < 0
			|| this->pri < 0
			|| this->deadline < 0
			|| this->io < 0) {
		return false;
	} else {
		return true;
	}
}

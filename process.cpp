#include "process.hpp"

//Pid	Bst	Arr	Pri Ddline	I/O

Process::Process(){}

Process::Process(int pid, int bst, int arr, int pri, int ddline, int io) {
	this->pid = pid;
	this->burst = bst;
	this->arr = arr;
	this->pri = pri;
	this->deadline = ddline;
	this->io = io;
}

Process::~Process() {
	
}

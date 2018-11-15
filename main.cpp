#include <iostream>
#include <fstream>
#include "process.hpp"
#include <string>
using namespace std;

int main(int argc, char* argv[]) {

    ifstream file;
    file.open (argv[1]);
    string line;

   if (file.is_open()) {
    Process* p = new Process();

    while (file >> p->pid >> p->arr >> p->burst >> p->pri >> p->deadline >> p->io) {
       p->print();
           }
    file.close();
  }



    return 0;
}


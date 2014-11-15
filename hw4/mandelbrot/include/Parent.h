#ifndef PARENTH
#define PARENTH

#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <limits>

#include <cstring>
#include "util.h"

using namespace std;

class Parent
{
  public:
    Parent(int mid, int qid1, int qid2, int c1, int c2, int pipefd[2]);
    ~Parent();
    void start();
  protected:
  
  private:
    int c1;
    int c2;
    int mid;
    int qid1; //input
    int qid2; //output
    FILE* pipe;
    
    void write(char* message);
};

#endif
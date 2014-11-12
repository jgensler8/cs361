#ifndef CHILD_TWOH
#define CHILD_TWOH

#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <unistd.h>
#include "util.h"

using namespace std;

class Child_Two
{
  public:
    Child_Two(int memid, int qid1, int qid2);
    void start();
  protected:
  
  private:
    char* memid;
    char* qid1;
    char* qid2;
};

#endif
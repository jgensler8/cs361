#ifndef CHILD_ONEH
#define CHILD_ONEH

#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <unistd.h>
#include "util.h"

using namespace std;

class Child_One
{
  public:
    Child_One(int memid, int qid);
    void start();
  protected:
  
  private:
    char* memid;
    char* qid;
};

#endif
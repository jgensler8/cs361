#ifndef UTILH
#define UTILH

#include <iostream>
#include <cstdlib>
#include <cstdio>

namespace util
{
  const int QMSGSIZE = 50;
  const int SHAREDMEMORYID = 0x1234;
  
  char* intToString(int i);
  void printName();
  
  void sigCHLDcallback(int status);
  void sigUSR1callback(int status);
  
  struct qmsg {
    long type;
    char message[QMSGSIZE];
  };
}

#endif
#ifndef UTILH
#define UTILH

#include <iostream>
#include <cstdlib>
#include <cstdio>

namespace util
{
  const int SHAREDMEMORYID = 0x1234;
  
  char* intToString(int i);
  void printName();
}

#endif
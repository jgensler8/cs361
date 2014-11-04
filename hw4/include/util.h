#ifndef UTILH
#define UTILH

#include <iostream>

namespace util
{
  extern void printCreds();
  
  extern void sigusr2(int signal);
  extern void sigusr1(int signal);
}

#endif
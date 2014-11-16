#ifndef UTILH
#define UTILH

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace util
{
  //utility
  const int NUMCOLORS = 15;
  const int MAXFILENAME = 50;
  const char COLORS[NUMCOLORS] = {
    '.','-','~',':','+',
    '*','%','O','8','&',
    '?','$','@','#','X' };
  const int COLUMNWIDTH = 8;
  char* intToString(int i);
  void printName();
  void initializeInput(double &xMin, double &xMax, double &yMin, double &yMax, int &nRows, int &nCols, int &maxIters);
  bool isValidInput(double xMin, double xMax, double yMin, double yMax, int nRows, int nCols, int maxIters);
  char getColor(int n);
  
  //shared memory
  const int MEMKEY = 0x73FF;
  const int MEMSIZE = 1000000; //1,000,000
  
  //signals
  int setCHLDhandler( void(* func)(int) );
  int setUSR1handler( void(* func)(int) );
  
  //messages queues
  const int QKEY1 = 0xDEAD;
  const int QKEY2 = 0xBEEF;
  const int QMSGSIZE = 50;
  const int MSGTYPE1 = 0x1;
  struct qmsg {
    long type;
    char message[QMSGSIZE];
  };
  int msg_snd(int qid, char* message);
  int msg_rcv(int qid, char* &message);
}

#endif
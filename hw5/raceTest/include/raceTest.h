#ifndef RACETESTH
#define RACETESTH

#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "util.h"

typedef struct struct_threadInfo
{
  int nBuffers;
  int workerID;
  double sleepTime;
  int semID;
  int mutexID;
  int *buffers;
  int nReadErrors;
  //additional
  int currentOpIndex;
  int useLocks;
  
} threadInfo;

extern void rt_parseArgs(int, char**, int*, int*, double*, double*, int*, int*);
extern int rt_isValidInput(int, int, double, double, int );
extern void rt_srand(int);
extern void rt_initDoubles(double*, int, double, double);
extern void rt_initSemaphores(int, int);
extern void rt_initThreadInfo( threadInfo*, int, double*, int*, int, int, int);
extern void rt_countRWErrors(threadInfo*, int, int*, int*, int*, int);

//thread
extern void* rt_worker( void*);
extern void rt_startThreads( threadInfo*, int);

#endif
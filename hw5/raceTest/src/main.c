#include "util.h"
#include "raceTest.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>


#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#else
/* according to X/OPEN we have to define it ourselves */
union semun {
  int val;                /* value for SETVAL */
  struct semid_ds *buf;   /* buffer for IPC_STAT, IPC_SET */
  unsigned short *array;  /* array for GETALL, SETALL */
  
  /* Linux specific part: */
  struct seminfo *__buf;  /* buffer for IPC_INFO */
};
#endif

int main(int argc, char** argv)
{
  //print credentials
  util_printName();
  
  //variables
  int nBuffers, nWorkers, randSeed = 0, useLocks = 0;
  double sleepMin, sleepMax;
  
  rt_parseArgs(argc, argv, &nBuffers, &nWorkers, &sleepMin, &sleepMax, &randSeed, &useLocks);
  if(!rt_isValidInput(nBuffers, nWorkers, sleepMin, sleepMax, randSeed))
  {
    printf("Invalid command line arguments. Exiting...");
    exit(-1);
  }
  
  //seed random numberGenerator
  rt_srand(randSeed);
  //create semaphore
  int semID = semget(UTIL_SEMKEY, nBuffers + 1, IPC_PRIVATE | IPC_CREAT | 0666);
  if( semID == -1)
  {
    printf("Could not allocated the semaphore set. Exiting...");
    exit(-1);
  }
  rt_initSemaphores(semID, nBuffers);
  //create nBuffers
  int* buffers= (int*)malloc( sizeof(int)*nBuffers);
  //create m doubles
  double* doubles = (double*)malloc( sizeof(double)*nWorkers);
  
  rt_initDoubles(doubles, nWorkers, sleepMin, sleepMax);
  qsort(doubles, nWorkers, sizeof(double), util_doublecomparator);
  
  threadInfo* threads = (threadInfo*)malloc( sizeof(struct struct_threadInfo)*nWorkers);
  rt_initThreadInfo(threads, nWorkers, doubles, buffers, nBuffers, semID, useLocks);
  
  printf("STARTING THREADS\n");
  rt_startThreads(threads, nWorkers);
  printf("DONE WITH THREADS\n");
  
  //count the types of errors
  int totalReadErrors, totalWriteErrors;
  rt_countRWErrors(threads, nWorkers, &totalReadErrors, &totalWriteErrors, buffers, nBuffers);
  printf("Read Errors: %d\nWrite Errors: %d\n", totalReadErrors, totalWriteErrors);
  
  union semun arg;
  arg.val = 0;
  semctl(semID, 0, IPC_RMID, arg);
  free(threads);
  free(buffers);
  return 0;
}
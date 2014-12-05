#include "raceTest.h"

void rt_parseArgs(int argc, char** argv, int* nBuffers, int* nWorkers, double* sleepMin, double* sleepMax, int* randSeed, int* useLocks)
{
  if(argc < 5)
  {
    printf("Invalid number of command line arguements. Exiting...\n");
    exit(-1);
  }
  
  //mandatory args
  *nBuffers = atoi(argv[1]);
  *nWorkers = atoi(argv[2]);
  *sleepMin = atof(argv[3]);
  *sleepMax = atof(argv[4]);
  
  //optional args
  if(argc == 6)
  {
    *randSeed = atoi(argv[5]); // will deafult to 0 if not able to parse, so safe
    if( NULL != strstr("-lock", argv[5])) *useLocks = 1;
    if( NULL != strstr("-nolock", argv[5])) *useLocks = 0;
  }
  else if(argc > 6)
  {
    *randSeed = atoi(argv[5]); // will deafult to 0 if not able to parse, so safe
    if( NULL != strstr("-lock", argv[6])) *useLocks = 1;
    if( NULL != strstr("-nolock", argv[6])) *useLocks = 0;
  }
}

int rt_isValidInput(int nBuffers, int nWorkers, double sleepMin, double sleepMax, int randSeed)
{
  if( !util_isPrime(nBuffers) || nBuffers < 1 || nBuffers > 32) return 0;
  if( nWorkers >= nBuffers || nWorkers < 1) return 0;
  if( sleepMin <= 0) return 0;
  if( sleepMax <= 0) return 0;
  if( sleepMax < sleepMin) return 0;
  
  return 1;
}

void rt_srand(int seed)
{
  if(seed == 0) srand(time(NULL));
  else srand(seed);
}

void rt_initDoubles(double* arr, int len, double low, double hi)
{
  int n;
  for(n = 0; n < len; ++n)
  {
    arr[n] = ((rand()*1.0)/RAND_MAX)*(hi-low) + low;
  }
}

void rt_initSemaphores(int semID, int len)
{
  int n;
  union semun arg;
  arg.val = 1;
  for(n = 0; n < len; ++n)
  {
    if(semctl(semID, n, SETVAL, arg) == -1)
    {
      printf("failed to initialize semaphore %d to value %d\n", n, arg.val);
    }
  }
}

void rt_initThreadInfo(threadInfo* arr, int len, double* doubles, int* buffers, int bufferslen, int semID, int useLocks)
{
  int n;
  for(n = 0; n < len; ++n)
  {
    arr[n].nBuffers = bufferslen;
    arr[n].workerID = n + 1;
    arr[n].sleepTime = doubles[n];
    arr[n].semID = semID;
    arr[n].mutexID = -1;
    arr[n].buffers = buffers;
    arr[n].nReadErrors = 0;
    //additional
    arr[n].currentOpIndex = n;
    arr[n].useLocks = useLocks;
    
    printf("sleeptime[%d] = %lf\n", n, arr[n].sleepTime);
  }
}

int rt_fetch(int* buffers, int len, int index)
{
  return buffers[index];
}

void rt_store(int* buffers, int len, int index, int val)
{
  buffers[index] = val;
}

void rt_updateOpIndex(threadInfo* info)
{
  info->currentOpIndex = (info->currentOpIndex + info->workerID) % info->nBuffers;
}

void rt_reportBadBits(int expected, int actual)
{
  int toCheck = 0x01;
  int i = 0;
  while(toCheck > 0)
  {
    if((toCheck & expected) != (toCheck & actual))
    {
      printf("%d ", i);
    }
    
    toCheck = toCheck << 1;
    ++i;
  }
}

//========== MUTEX ==========
void rt_acquire(int semID, int index)
{
  //acquire lock
  struct sembuf arg;
  arg.sem_num = index;
  arg.sem_op = -1;
  arg.sem_flg = SEM_UNDO;
  if(semop(semID, &arg, 1) == -1)
  {
    printf("ERROR acquiring semaphore %d\n", index);
  }
}

void rt_release(int semID, int index)
{
  //release lock
  struct sembuf arg;
  arg.sem_num = index;
  arg.sem_op = 1;
  arg.sem_flg = SEM_UNDO;
  if(semop(semID, &arg, 1) == -1)
  {
    printf("ERROR releasing semaphore %d\n", index);
  }
}

//========== NON SEM OPS ==========
void rt_readOP(threadInfo* info)
{
  int before = rt_fetch(info->buffers, info->nBuffers, info->currentOpIndex);
  usleep(info->sleepTime);
  int after = rt_fetch(info->buffers, info->nBuffers, info->currentOpIndex);
  
  if(before != after)
  {
    rt_acquire(info->semID, 0);
    printf("Worker %d observed that buffer %d changed from %d to %d ( Bad bits = ", info->workerID, info->currentOpIndex, before, after);
    rt_reportBadBits(before, after);
    printf(" )\n");
    rt_release(info->semID, 0);
    info->nReadErrors += 1;
  }
}

void rt_writeOP(threadInfo* info)
{
  int before = rt_fetch(info->buffers, info->nBuffers, info->currentOpIndex);
  usleep(info->sleepTime);
  rt_store(info->buffers, info->nBuffers, info->currentOpIndex, before + (0x01 << (info->workerID-1)));
}

//========== SEM OPS ==========
void rt_readOP_sem(threadInfo* info)
{
  //acquire lock
  rt_acquire(info->semID, info->currentOpIndex);
  int before = rt_fetch(info->buffers, info->nBuffers, info->currentOpIndex);
  //usleep(info->sleepTime);
  int after = rt_fetch(info->buffers, info->nBuffers, info->currentOpIndex);
  
  //release lock
  rt_release(info->semID, info->currentOpIndex);
  
  if(before != after)
  {
    rt_acquire(info->semID, info->nBuffers);
    printf("Worker %d observed that buffer %d changed from %d to %d ( Bad bits = ", info->workerID, info->currentOpIndex, before, after);
    rt_reportBadBits(before, after);
    printf(")\n");
    rt_release(info->semID, info->nBuffers);
    info->nReadErrors += 1;
  }
}

void rt_writeOP_sem(threadInfo* info)
{
  //acuquire lock
  rt_acquire(info->semID, info->currentOpIndex);
  
  int before = rt_fetch(info->buffers, info->nBuffers, info->currentOpIndex);
  //usleep(info->sleepTime);
  rt_store(info->buffers, info->nBuffers, info->currentOpIndex, before + (0x01 << (info->workerID-1)));
  //release lock
  rt_release(info->semID, info->currentOpIndex);
}


//========== WORKER THREAD FUNCTION ==========
void* rt_worker(void* infoVoid)
{
  //cast parameters
  threadInfo* info = (threadInfo*) infoVoid;
  
  //acquire screen mutex?
  //info->buffers[info->workerID] = info->workerID;
  //printf("Worker %d, sleepTime: %lf, buffers: %p\n", info->workerID, info->sleepTime, info->buffers);
  //fflush(stdout);
  
  if(info->useLocks == 0)
  {
    int n;
    for(n = 0; n < info->nBuffers; ++n)
    {
      //read
      rt_readOP(info);
      rt_updateOpIndex(info);
      //read
      rt_readOP(info);
      rt_updateOpIndex(info);
      
      //write
      rt_writeOP(info);
      rt_updateOpIndex(info);
    }
  }
  else if (info->useLocks == 1)
  {
    int n;
    for(n = 0; n < info->nBuffers; ++n)
    {
      //read
      rt_readOP_sem(info);
      rt_updateOpIndex(info);
      //read
      rt_readOP_sem(info);
      rt_updateOpIndex(info);
      
      //write
      rt_writeOP_sem(info);
      rt_updateOpIndex(info);
    }
  }
  
  return NULL;
}

void rt_startThreads(threadInfo* arr, int len)
{
  pthread_t threads[len];
  
  int n;
  for(n = 0; n < len; ++n)
  {
    pthread_create(&threads[n], NULL, rt_worker, (void*) &arr[n]);
  }
  
  for(n = 0; n < len; ++n)
  {
    pthread_join(threads[n], NULL);
  }
}

void rt_countRWErrors(threadInfo* threads, int threadslen, int* readErrors, int* writeErrors, int* buffers, int bufferslen)
{
  int n,
    expectedResult = (0x01 << threadslen) - 1;
  printf("Expected result: %d\n", expectedResult);
  *readErrors = 0;
  *writeErrors = 0;
  
  for(n = 0; n < threadslen; ++n)
  {
    *readErrors += threads[n].nReadErrors;
  }
  for(n = 0; n < bufferslen; ++n)
  {
    printf("Buffer %d holds %d", n, buffers[n]);
    if( buffers[n] != expectedResult)
    {
      printf(" (Bad Bits = ");
      rt_reportBadBits(buffers[n], expectedResult);
      printf(")");
      *writeErrors += 1;
    }
    printf("\n");
  }
}
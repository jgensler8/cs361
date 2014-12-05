#ifndef UTILH
#define UTILH

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


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

//constants
static const int UTIL_SEMKEY = 0x1337;

//utility
extern char* util_intToString(int i);
extern void util_printName();
extern int util_doublecomparator(const void* a, const void* b);
extern int util_isPrime(int);

#endif
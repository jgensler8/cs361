#include "util.h"

//utility
char* util_intToString(int i)
{
  int len = 0, temp = i;
  
  if(temp < 0) temp *= -1;
  
  while( temp > 0)
  {
    temp /= 10;
    ++len;
  }
  
  char* s = (char*)malloc(sizeof(char)*(len+1));
  sprintf(s,"%d",i);
  s[len] = '\0';
  
  return s;
}

void util_printName()
{
  printf("Jeff Gensler, jgensler, jgensl2@uic.edu, HW4\n");
}

int util_doublecomparator(const void* a, const void* b)
{
  if( *(double*)a < *(double*)b) return 1;
  else if (*(double*)a > *(double*)b) return -1;
  else return 0;
}

int util_isPrime(int num)
{
  int n;
  for(n = 2; n < num/2 ; ++n)
  {
    if((num % n) == 0) return 0;
  }
  return 1;
}
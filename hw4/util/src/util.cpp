#include "util.h"

namespace util
{
  
  char* intToString(int i)
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
  
  void printName()
  {
    using namespace std;
    std::cout << "Jeff Gensler, jgensler, jgensl2@uic.edu, HW4" << std::endl << std::flush;
  }
 
  void sigCHLDcallback(int status)
  {
    exit(-1);
  }
  
  void sigUSR1callback(int status)
  {
    exit(-1);
  }
}
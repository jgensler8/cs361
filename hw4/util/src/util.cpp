#include "util.h"

namespace util
{
  //utility
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
 
  void initializeInput(double &xMin, double &xMax, double &yMin, double &yMax, int &nRows, int &nCols, int &maxIters)
  {
    xMin = -1;
    xMax = -1;
    yMin = -1;
    yMax = -1;
    nRows = -1;
    nCols = -1;
    maxIters = -1;
  }
 
  bool isValidInput(double xMin, double xMax, double yMin, double yMax, int nRows, int nCols, int maxIters)
  {
    //if( xMin < 0) return false;
    //if( xMax < 0) return false;
    //if( yMin < 0) return false;
    //if( yMax < 0) return false;
    if( nRows < 0) return false;
    if( maxIters < 0) return false;
    return true;
  }
 
  //signals
  void sigCHLDcallback(int status)
  {
    exit(-1);
  }
  
  void sigUSR1callback(int status)
  {
    exit(-1);
  }
  
  
  //messages
  int msg_snd(int qid, char* message)
  {
    util::qmsg buf;
    buf.type = util::MSGTYPE1;
    strncpy(buf.message, message, util::QMSGSIZE);
    buf.message[util::QMSGSIZE-1] = '\0';           //ensure we don't send a weird string across
    return msgsnd(qid, &buf, util::QMSGSIZE , 0);
    //return msgsnd(qid, &buf, strlen(buf.message)+1 , 0);
  }
  
  int msg_rcv(int qid, char* &message)
  {
    util::qmsg buf;
    int size = msgrcv(qid, &buf, util::QMSGSIZE, util::MSGTYPE1, 0);
    message = (char*)malloc(sizeof(char)*(1+strlen(buf.message)));
    strcpy(message, buf.message);
    return size;
  }
}
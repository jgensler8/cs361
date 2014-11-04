#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "util.h"

using namespace std;

int main()
{
  util::printCreds();
  
  //create pipes
  int p_c1 = pipe(0);
  int c1_c2 = pipe(0);
  
  //create message queues
  int p_c1_c2 = msgget(0, IPC_CREAT);
  int p_c2 = msgget(1, IPC_CREAT);
  
  //set signal handler for sigchild
  
  //fork child 1
      //close pipe
      
  //for child 2
      //close pipe

  
  //step 7
  
  //report final resultss
  
  std::cout << "DONE!" << std::endl;
  return 0;
}
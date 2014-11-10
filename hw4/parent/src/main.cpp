#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "Parent.h"
#include "util.h"

using namespace std;
using namespace util;

//fork and run the child one process
void forkChildOne(int mem, int fd)
{
  int child = fork();
  if( child < 0)
  {
    std::cout << "error forking child one" << std::endl;
    exit(-1);
  }
  else if (child == 0)
  {
    //child one
    char* command[] = {"chid_one", util::intToString(mem), util::intToString(fd), NULL};
    execve("child_one", command, NULL);
    exit(-1);
  }
}

//fork and run the child two process
void forkChildTwo(int mem, int fd)
{
  int child = fork();
  if( child < 0)
  {
    std::cout << "error forking child two" << std::endl;
    exit(-1);
  }
  else if (child == 0)
  {
    //child two
    char * command[] = {"chid_two", util::intToString(mem), util::intToString(fd), NULL};
    execve("child_two", command, NULL);
    exit(-1);
  }
}



int main()
{
  util::printName();
  
  //create pipes
  int
    pipe_p_c1_fd[2] = { 0, 1},
    pipe_p_c1 = pipe( pipe_p_c1_fd),
    pipe_c1_c2_fd[2] = { 0, 1},
    pipe_c1_c2 = pipe( pipe_c1_c2_fd);
  
  //create message queues
  
  //fork children
  forkChildOne(10,10);
  forkChildTwo(10,10);
  
  return 0;
}
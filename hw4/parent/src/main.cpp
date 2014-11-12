#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "Parent.h"
#include "Child_One.h"
#include "Child_Two.h"
#include "util.h"

using namespace std;
using namespace util;

#define OPEN 1
#define CLOSE 0
#define STDIN_FD 0
#define STDOUT_FD 1

//fork and run the child one process
void forkChildOne(int memid, int qid, int pipe_p_c1_fd[2], int pipe_p_c1, int pipe_c1_c2_fd[2], int pipe_c1_c2)
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
    //close unused pipe ends
    close(pipe_p_c1_fd[1]);
    close(pipe_c1_c2_fd[0]);
    
    
    //redirect pipes
    dup2(pipe_p_c1_fd[0], STDIN_FD);
    dup2(pipe_c1_c2_fd[1], STDOUT_FD);
    
    //start the child
    Child_One* child = new Child_One(memid, qid);
    child->start();
    exit(-1);
  }
}

//fork and run the child two process
void forkChildTwo(int memid, int qid1, int qid2, int pipe_p_c1_fd[2], int pipe_p_c1, int pipe_c1_c2_fd[2], int pipe_c1_c2)
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
    //close unused pipe ends
    close(pipe_c1_c2_fd[1]);
    close(pipe_p_c1_fd[0]);
    close(pipe_p_c1_fd[1]);
    
    //redirect pipes
    dup2(pipe_c1_c2_fd[0], STDIN_FD);
    
    //start the child
    Child_Two* child = new Child_Two(memid, qid1, qid2);
    child->start();
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
  int
    mq1 = 1,
    mq2 = 2;
  
  //create shared memory
  int memID = 10;
  
  //fork children
  forkChildOne(memID, mq1, pipe_p_c1_fd, pipe_p_c1, pipe_c1_c2_fd, pipe_c1_c2);
  forkChildTwo(memID, mq1, mq2, pipe_p_c1_fd, pipe_p_c1, pipe_c1_c2_fd, pipe_c1_c2);
  
  //close unused pipes
  close(pipe_c1_c2_fd[0]);
  close(pipe_c1_c2_fd[1]);
  close(pipe_p_c1_fd[0]);
  
  //redirect pipes
  dup2(pipe_p_c1_fd[1], STDOUT_FD);
  
  Parent* parent = new Parent(memID, mq1, mq2);
  parent->start();
  
  return 0;
}
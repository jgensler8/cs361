#include "Parent.h"


Parent::Parent(int mid, int qid1, int qid2, int c1, int c2, int pipefd[2])
{
  this->mid = mid;
  this->qid1 = qid1;
  this->qid2 = qid2;
  this->c1 = c1;
  this->c2 = c2;
  this->pipe = fdopen(pipefd[1], "w"); //write end of the pipe
}

Parent::~Parent()
{
  //close file descriptor?
}

void Parent::start()
{
  char* filename = (char*)malloc(sizeof(char)*util::MAXFILENAME);
  double xMin, xMax, yMin, yMax;
  int nRows, nCols, maxIters;
  
  //CHLD handler
  //util::setCHLDhandler(util::CHLDhandler);
  
  while(true)
  {
    fprintf(stdout, "Enter filenamme:");
    fscanf(stdin, "%s", filename);
    fprintf(stdout, "Enter xMin:");
    fscanf(stdin, "%lf", &xMin);
    fprintf(stdout, "Enter xMax:");
    fscanf(stdin, "%lf", &xMax);
    fprintf(stdout, "Enter yMin:");
    fscanf(stdin, "%lf", &yMin);
    fprintf(stdout, "Enter yMax:");
    fscanf(stdin, "%lf", &yMax);
    fprintf(stdout, "Enter nRows:");
    fscanf(stdin, "%d", &nRows);
    fprintf(stdout, "Enter nCols:");
    fscanf(stdin, "%d", &nCols);
    fprintf(stdout, "Enter maxIters:");
    fscanf(stdin, "%d", &maxIters);
    
    fflush(stdin);
    
    if( ! util::isValidInput(xMin, xMax, yMin, yMax, nRows, nCols, maxIters))
    {
      fprintf(stdout, "Sorry, that input was invalid. Try again.\n");
      fflush(stdout);
      fflush(stdin);
      continue;
    }
    
    //send data over the pipe to child one
    fprintf(this->pipe, "%lf %lf %lf %lf %d %d %d \n", xMin, xMax, yMin, yMax, nRows, nCols, maxIters);
    fflush(this->pipe);
    
    //send the filename over the q1 to child two
    util::msg_snd(this->qid2, filename);
    
    //listen for done from child one
    char* message_c1 = NULL;
    util::msg_rcv(this->qid1, message_c1);
    
    //listen for done from child two
    char* message_c2 = NULL;
    util::msg_rcv(this->qid1, message_c2);
    
    free(message_c1);
    free(message_c2);
    
    //prompt user for more input
    char yesNo;
    fprintf(stdout, "Would you like to enter more? 'Y' or 'y' for yes, some char for no :");
    fscanf(stdin, " %c", &yesNo);
    if( yesNo != 'Y' && yesNo != 'y')
    {
      break;
    }
    fflush(stdin);
  }
  
  fprintf(stdout, "Goodbye!\n");
  fflush(stdout);
}
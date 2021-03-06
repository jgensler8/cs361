#include "mandelDisplay.h"

#include <iostream>
#include <cstdlib>
#include <limits>
#include "util.h"

#include <fstream>

using namespace std;

int numCalculations = 0;

void parseArgs(int argc, char** argv, int &memid, int &qid1, int &qid2)
{
  if( argc >= 3)
  {
    memid = atoi( argv[1]);
    qid1 = atoi( argv[2]);
    qid2 = atoi( argv[3]);
  }
}

void calcUSR1handler(int status)
{
  exit(numCalculations);
}

int main(int argc, char** argv)
{
  //parse args
  int memid = 0, qid1 = 0, qid2 = 0;
  parseArgs(argc, argv, memid, qid1, qid2);
  
  //std::cout << "args:"
  //  << qid1 << " "
  //  << qid2 << std::endl;
  
  //set signal handler
  util::setUSR1handler(calcUSR1handler);
  
  double xMin, xMax, yMin, yMax;
  int nRows, nCols, maxIters;
  
  while(true)
  {
    
    //some weird multiple input bug...
    util::initializeInput(xMin, xMax, yMin, yMax, nRows, nCols, maxIters);
    
    fscanf(stdin, " %lf %lf %lf %lf %d %d %d", &xMin, &xMax, &yMin, &yMax, &nRows, &nCols, &maxIters);
      
    //some weird multiple input bug...
    if( ! util::isValidInput(xMin, xMax, yMin, yMax, nRows, nCols, maxIters))
    {
      fprintf(stdout, "MANDELDISPLAY GOT INVALID INPUT\n");
      fflush(stdout);
      fflush(stdin);
      exit(-999);
    }
    
    //receive file
    char* filename;
    util::msg_rcv(qid2, filename);
    //open file
    ofstream outfile;
    outfile.open (filename);
    
    //calculate the display function
    mandelDisplay* m = new mandelDisplay(xMin, xMax, yMin, yMax, nRows, nCols, maxIters, memid, outfile);
    int result = m->display();
    delete m;
    char* ch_result = util::intToString(result);
    
    //close file
    outfile.close();
    
    //write these to stdout
    fprintf(stdout, "%lf %lf %lf %lf %d %d %d\n", xMin, xMax, yMin, yMax, nRows, nCols, maxIters);
    
    ++numCalculations;
      
    //write done to message queue
    util::msg_snd(qid1, ch_result);
    
    free(ch_result);
    free(filename);
  }
  
  return 0;
}
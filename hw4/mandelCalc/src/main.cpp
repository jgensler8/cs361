#include "mandelCalc.h"

#include <iostream>
#include <cstdlib>
#include <limits>
#include "util.h"

#include <fstream>

using namespace std;

void parseArgs(int argc, char** argv, int &memid, int &qid)
{
  if( argc >= 2)
  {
    memid = atoi( argv[1]);
    qid = atoi( argv[2]);
  }
}

int main(int argc, char** argv)
{
  //parse args
  int memid = 0, qid = 0;
  parseArgs(argc, argv, memid, qid);
  
  //set signal handler
  util::setUSR1handler();
  
  double xMin, xMax, yMin, yMax;
  int nRows, nCols, maxIters;
  
  while(true)
  {
    util::initializeInput(xMin, xMax, yMin, yMax, nRows, nCols, maxIters);
    
    fscanf(stdin, " %lf %lf %lf %lf %d %d %d", &xMin, &xMax, &yMin, &yMax, &nRows, &nCols, &maxIters);
    
    if( ! util::isValidInput(xMin, xMax, yMin, yMax, nRows, nCols, maxIters))
    {
      //fprintf(stdout, "MANDELCALC ERROR\n");
      //fprintf(stdout, "%lf %lf %lf %lf %d %d %d", xMin, xMax, yMin, yMax, nRows, nCols, maxIters);
      fflush(stdin);
      fflush(stdout);
      //break;
      //continue;
      exit(-1);
    }
    
    mandelCalc* m = new mandelCalc(xMin, xMax, yMin, yMax, nRows, nCols, maxIters, memid);
    int result = m->mandelbrot();
    delete m;
    char* ch_result = util::intToString(result);
    
    //write these to stdout
    fprintf(stdout, "%lf %lf %lf %lf %d %d %d\n", xMin, xMax, yMin, yMax, nRows, nCols, maxIters);
    fflush(stdout);
      
    //write done to message queue
    util::msg_snd(qid, ch_result);
    
    free(ch_result);
  }
  
  return 0;
}
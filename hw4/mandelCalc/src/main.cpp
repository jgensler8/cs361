#include "mandelCalc.h"

#include <iostream>
#include <cstdlib>
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
  
  //testing
  ofstream testfile;
  testfile.open ("mandelCalc.output");
  
  while(std::cin.good())
  {
    //some weird multiple input bug...
    util::initializeInput(xMin, xMax, yMin, yMax, nRows, nCols, maxIters);
    
    std::cin
      >> xMin
      >> xMax
      >> yMin
      >> yMax
      >> nRows
      >> nCols
      >> maxIters;
    
    //some weird multiple input bug...
    if( ! util::isValidInput(xMin, xMax, yMin, yMax, nRows, nCols, maxIters)) continue;
    
    mandelCalc* m = new mandelCalc(xMin, xMax, yMin, yMax, nRows, nCols, maxIters, memid);
    int result = m->mandelbrot();
    delete m;
    char* ch_result = util::intToString(result);
    
    //testing
    testfile << "RESULT:" << ch_result << std::endl;
    
    //write these to stdout
    std::cout
      << xMin << " "
      << xMax << " "
      << yMin << " "
      << yMax << " "
      << nRows << " "
      << nCols << " "
      << maxIters << std::endl << std::flush;
      
    //write done to message queue
    util::msg_snd(qid, ch_result);
    
    free(ch_result);
  }
  
  
    testfile.close();
  
  return 0;
}
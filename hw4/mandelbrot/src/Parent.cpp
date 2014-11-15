#include "Parent.h"

#include <fstream>

Parent::Parent(int mid, int qid1, int qid2, int c1, int c2)
{
  this->mid = mid;
  this->qid1 = qid1;
  this->qid2 = qid2;
  this->c1 = c1;
  this->c2 = c2;
}

void Parent::start()
{
  std::string filename;
  double xMin, xMax, yMin, yMax;
  int nRows, nCols, maxIters;
  
  //testing
  ofstream testfile;
  testfile.open ("mandelbrot.output");
  
  while(std::cin.good())
  {
    std::cin
      >> filename
      >> xMin
      >> xMax
      >> yMin
      >> yMax
      >> nRows
      >> nCols
      >> maxIters;
    //send data over the pipe to child one
    
    std::cout
      << xMin << " "
      << xMax << " "
      << yMin << " "
      << yMax << " "
      << nRows << " "
      << nCols << " "
      << maxIters << std::endl << std::flush;
    
    //send the filename over the q1 to child two
    util::msg_snd(this->qid2, (char*)filename.c_str());
    
    //listen for done from child one
    char* message_c1 = NULL;
    util::msg_rcv(this->qid1, message_c1);
    
    //listen for done from child two
    char* message_c2 = NULL;
    util::msg_rcv(this->qid1, message_c2);
    
    //testing
    testfile << "DONE READING MESSAGE:"
      << this->qid1 << " "
      << this->qid2 << " "
      << this->mid << " "
      << message_c1 << " "
      << message_c2 << std::endl;
    
    free(message_c1);
    free(message_c2);
  }
  
  testfile.close();
  
  //send signals to children
  //TODO
  
  //wait for children
  //TODO
}
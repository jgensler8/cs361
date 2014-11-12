#include "Parent.h"

Parent::Parent(int mid, int qid1, int qid2)
{
  this->mid = mid;
  this->qid1 = qid1;
  this->qid2 = qid2;
}

void Parent::start()
{
  std::string filename, xMin, xMax, yMin, yMax, nRows, nCols, maxIters;
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
    std::cout << xMin << xMax << yMin << yMax << nRows << nCols << maxIters;
    
    //send the filename over the q1 to child two
    util::qmsg message;
    strncpy(message.message, filename.c_str(), util::QMSGSIZE);
    msgsnd(this->qid1, &message, util::QMSGSIZE, 0);
    
    //listen for done from child one
    
    //listen for done from child two
    msgrcv()
  }
}
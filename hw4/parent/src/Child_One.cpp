#include "Child_One.h"

Child_One::Child_One(int memid, int qid)
{
  this->memid = util::intToString(memid);
  this->qid = util::intToString(qid);
}

void Child_One::start()
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
      
    
  }
  //execlp("./mandelCalc", "mandelCalc", this->memid, this->qid, NULL);
}
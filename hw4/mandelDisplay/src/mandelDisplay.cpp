#include "mandelDisplay.h"

mandelDisplay::mandelDisplay(double xMin, double xMax, double yMin, double yMax, int nRows, int nCols, int maxIters, int memID, std::ofstream &outfile) : outfile( outfile)
{
  this->xMin = xMin;
  this->xMax = xMax;
  this->yMin = yMin;
  this->yMax = yMax;
  this->nRows = nRows;
  this->nCols = nCols;
  this->maxIters = maxIters;
  this->memID = memID;
  this->memAddr = shmat(memID, 0, 0);
  //this->outfile = outfile;
}

mandelDisplay::~mandelDisplay()
{
  shmdt(this->memAddr);
}

template<typename T> T mandelDisplay::readValue(int r, int c, T &val)
{
  val = *((T*)this->memAddr + r * this->nCols + c);
  return val;
}

int mandelDisplay::display()
{
  for(int r = this->nRows - 1; r >= 0; --r)
  {
    //print first column
    if(r == this->nRows - 1)
    {
      std::cout << std::setw(util::COLUMNWIDTH) << this->yMin;
    }
    else if( r == 0)
    {
      std::cout << std::setw(util::COLUMNWIDTH) << this->yMax;
    }
    else
    {
      std::cout << std::setw(util::COLUMNWIDTH) << "";
    }
    
    //print data
    for(int c = this->nCols - 1; c >= 0; --c)
    {
      int n;
      this->readValue<int>(r,c,n);
      if( n < 0)
      {
        std::cout << " ";
      }
      else
      {
        std::cout << util::getColor(n);
      }
      this->outfile << n << " ";
    }
    std::cout << std::endl;
    this->outfile << std::endl;
  }
  
  return 0xB00C;
}
#include "mandelCalc.h"

mandelCalc::mandelCalc(double xMin, double xMax, double yMin, double yMax, int nRows, int nCols, int maxIters, int memID)
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
}

mandelCalc::~mandelCalc()
{
  shmdt(this->memAddr);
}

template<typename T> void mandelCalc::storeVal(int r, int c, T val)
{
  *((T*)this->memAddr + r * this->nCols + c) = val;
}

int mandelCalc::mandelbrot()
{
  double
    deltaX = (this->xMax - this->xMin)/(this->nCols-1),
    deltaY = (this->yMax - this->yMin)/(this->nRows-1);
  
  for(int r = 0; r < this->nRows; ++r)
  {
    double Cy = this->yMin + (r*deltaY);
    for(int c = 0; c < this->nCols; ++c)
    {
      double
        Cx = this->yMin + (c*deltaX),
        Zx = 0,
        Zy = 0;
      int n;
      
      for(n = 0; n < this->maxIters; ++n)
      {
        if(Zx * Zx + Zy* Zy >= 4.0) break;
        double
          Zx_next = Zx * Zx  - Zy * Zy + Cx,
          Zy_next = 2.0 * Zx * Zy + Cy;
          
        Zx = Zx_next;
        Zy = Zy_next;
      }
      
      if( n >= this->maxIters) this->storeVal<int>(r, c, n);
      else this->storeVal<int>(r, c, n);
    }
  }
  return 0xFACE;
}
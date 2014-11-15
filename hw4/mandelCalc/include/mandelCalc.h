#ifndef MANDELCALCH
#define MANDELCALCH

#include <sys/types.h>
#include <sys/shm.h>

class mandelCalc
{
  public:
    mandelCalc(double xMin, double xMax, double yMin, double yMax, int nRows, int nCols, int maxIters, int memID);
    ~mandelCalc();
    int mandelbrot();
  protected:
  
  private:
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    int nRows;
    int nCols;
    int maxIters;
    int memID;
    void* memAddr;
    
    template<typename T> void storeVal(int r, int c, T val);
};

#endif
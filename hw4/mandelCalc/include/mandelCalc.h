#ifndef MANDELCALCH
#define MANDELCALCH

class mandelCalc
{
  public:
    mandelCalc(double xMin, double xMax, double yMin, double yMax, int nRows, int nCols, int maxIters);
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
};

#endif
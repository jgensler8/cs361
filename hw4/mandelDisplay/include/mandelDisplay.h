#ifndef MANDELDISPLAYH
#define MANDELDISPLAYH

class mandelDisplay
{
  public:
    mandelDisplay(double xMin, double xMax, double yMin, double yMax, int nRows, int nCols, int maxIters);
    int display();
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
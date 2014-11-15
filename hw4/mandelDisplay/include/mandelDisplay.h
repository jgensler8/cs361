#ifndef MANDELDISPLAYH
#define MANDELDISPLAYH

#include <sys/types.h>
#include <sys/shm.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "util.h"

using namespace std;

class mandelDisplay
{
  public:
    mandelDisplay(double xMin, double xMax, double yMin, double yMax, int nRows, int nCols, int maxIters, int memID, std::ofstream &outfile);
    ~mandelDisplay();
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
    
    std::ofstream &outfile;
    
    int memID;
    void* memAddr;
    
    template<typename T> T readValue(int r, int c, T &val);
};

#endif
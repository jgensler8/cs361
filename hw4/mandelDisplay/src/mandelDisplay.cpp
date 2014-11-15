#include "mandelDisplay.h"

mandelDisplay::mandelDisplay(double xMin, double xMax, double yMin, double yMax, int nRows, int nCols, int maxIters)
{
  this->xMin = xMin;
  this->xMax = xMax;
  this->yMin = yMin;
  this->yMax = yMax;
  this->nRows = nRows;
  this->nCols = nCols;
  this->maxIters = maxIters;
}

int mandelDisplay::display()
{
  return 555;
}
#include "Child_One.h"

Child_One::Child_One(int memid, int qid)
{
  this->memid = util::intToString(memid);
  this->qid = util::intToString(qid);
}

void Child_One::start()
{
  execlp("./mandelCalc", "mandelCalc", this->memid, this->qid, NULL);
}
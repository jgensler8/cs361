#include "Child_Two.h"

Child_Two::Child_Two(int memid, int qid1, int qid2)
{
  this->memid = util::intToString(memid);
  this->qid1 = util::intToString(qid1);
  this->qid2 = util::intToString(qid2);
}

void Child_Two::start()
{
  execlp("./mandelDisplay", "mandelDisplay", this->memid, this->qid1, this->qid2, NULL);
}
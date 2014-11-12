#include "Child_Two.h"

Child_Two::Child_Two(int memid, int qid1, int qid2)
{
  this->memid = util::intToString(memid);
  this->qid1 = util::intToString(qid1);
  this->qid2 = util::intToString(qid2);
}

void Child_Two::start()
{
  std::string word;
  std::cin >> word;
  std::cout << word << ".....From Child TWO" << std::endl;
  
  //execlp("./mandelDisplay", "mandelDisplay", this->m//emid, this->qid1, this->qid2, NULL);
}
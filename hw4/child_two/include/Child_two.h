#ifndef CHILD_TWOH
#define CHILD_TWOH

class Child_two
{
  public:
    Child_two(int pipe, int msgq1, int msgq2);
  protected:
  
  private:
    int pipe;
    int msgq1; //output
    int msgq2; //intput
};

#endif
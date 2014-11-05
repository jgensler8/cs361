#ifndef PARENTH
#define PARENTH

class Parent
{
  public:
    Parent(int pipe, int msgq1, int msgq2);
  protected:
  
  private:
    int pipe;
    int msgq1; //input
    int msgq2; //output
};

#endif
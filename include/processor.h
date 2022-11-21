#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float user;
  float nice;
  float idle;
  float system;
  float iowait;
  float irq;
  float softIrq;
  float steal;
  float puser = 0;
  float pnice = 0;
  float pidle = 0;
  float psystem = 0;
  float piowait = 0;
  float pirq = 0;
  float psoftIrq = 0;
  float psteal = 0;
};

#endif

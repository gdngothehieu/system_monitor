#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  enum cpuIndexValue {
	user = 0,
  	nice = 1,
  	system = 2,
  	idle = 3,
 	iowait = 4,
  	irq = 5,
  	softirq = 6,
  	steal = 7
  };
  float Utilization();
};

#endif
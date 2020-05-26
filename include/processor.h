#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

  // Declare necessary private members
 private:
    int previdle;
    int prevuser;
    int previowait;
    int prevnice;
    int previrq;
    int prevsystem;
    int prevsteal;
    int prevsoftirq;
};

#endif
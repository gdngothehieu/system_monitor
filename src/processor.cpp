#include "processor.h"
#include "linux_parser.h"
#include <vector>

using std::stof;
using std::vector;
using std::string;

float Processor::Utilization() { 
  vector<string> cpuUtils = LinuxParser::CpuUtilization();
  float idleCpus = stof(cpuUtils[cpuIndexValue::idle]) + stof(cpuUtils[cpuIndexValue::iowait]);
  float nonIdle = stof(cpuUtils[cpuIndexValue::user]) + stof(cpuUtils[cpuIndexValue::nice]) + stof(cpuUtils[cpuIndexValue::system]) + stof(cpuUtils[cpuIndexValue::irq]) + stof(cpuUtils[cpuIndexValue::softirq]) + stof(cpuUtils[cpuIndexValue::steal]);
  float total = idleCpus + nonIdle;
  return (total - idleCpus)/total;
}
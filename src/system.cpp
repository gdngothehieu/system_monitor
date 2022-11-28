#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using namespace std;

Processor& System::Cpu() { 
  static Processor cpu_;
  return cpu_; 
}

vector<Process>& System::Processes() { 
  static vector<Process> processes_;
  for(const int & pi: LinuxParser::Pids()){
  	Process process(pi);
    if(process.Command() != "" && process.Ram() != "")
    	processes_.emplace_back(Process(pi));
  }
  return processes_; 
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }

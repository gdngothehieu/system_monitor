#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid): pid_(pid) {}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { 
  return LinuxParser::CpuUtilization(pid_);
}

string Process::Command() { 
  const string command = LinuxParser::Command(pid_);
  int length = command.length();
  return length <= 40 ? command : command.substr(0, 40)+ "...";
}

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& otherProcess) const { return otherProcess.cpu_ < cpu_; }
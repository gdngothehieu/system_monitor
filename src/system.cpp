#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() {
  return cpu_;
}
vector<Process> returningpids = {};
// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  int size = pids.size();
  for(int i = 0;i<size;i++){
    Process placeholder(pids[i]);
    returningpids.push_back(placeholder);
  }
  vector<Process>& returning = returningpids;
  return returningpids;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
  //error when using l(s), system belies them to be 1(s), now using ns
  std::string ninux;
  std::string verson;
  std::string key;
  std::string inputline;
  std::string kernel = "/proc/version";
  std::ifstream fstream(kernel);
  if(fstream.is_open()){
    std::getline(fstream, inputline);
    std::istringstream str(inputline);
    str >> ninux >> verson >> key;
  }
  return key;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  string total;
  string free;
  string key;
  string value;

  string line;
  std::ifstream memstream("/proc/meminfo");
  if (memstream.is_open()) {
    while (std::getline(memstream, line)) {
      std::istringstream stream(line);
      while (stream >> key >> value) {
        if (key == "MemTotal:") {
          std::replace(value.begin(), value.end(), '_', ' ');
          total = value;
        }
        if(key=="MemFree:"){
          std::replace(value.begin(), value.end(), '_', ' ');
          free = value;
        }
        if(free!=""&&total!=""){
          return (std::stof(total)-std::stof(free))/std::stof(total);
        }
      }
    }
  }
  return 0.0;
}


// TODO: Return the operating system name
std::string System::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream("/etc/os-release");
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
  string line;
  string processes;
  string answer;
  std::ifstream processstream("/proc/stat");
  if (processstream.is_open()) {
    while (std::getline(processstream, line)) {
      std::istringstream stream(line);
      while (stream >> processes >> answer) {
        if (processes == "procs_running") {
          std::replace(answer.begin(), answer.end(), '_', ' ');
          return std::stoi(answer);
        }
      }
    }
  }
  return 0;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
  string line;
  string processes;
  string answer;
  std::ifstream processstream("/proc/stat");
  if (processstream.is_open()) {
    while (std::getline(processstream, line)) {
            std::istringstream stream(line);
      while (stream >> processes >> answer) {
        if (processes == "processes") {
          std::replace(answer.begin(), answer.end(), '_', ' ');
          return std::stoi(answer);
        }
      }
    }
  }
  return 0;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
  std::string uptime;
  std::string inputline;
  std::string from = "/proc/uptime";
  std::ifstream timestream(from);
  if(timestream.is_open()){
    std::getline(timestream, inputline);
    std::istringstream strem(inputline);
    strem >> uptime;
  }
  return std::stol(uptime, nullptr, 10);
}

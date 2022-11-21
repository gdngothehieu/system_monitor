#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {return pid_;}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  string b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,u,s,cu,cs,b18,b19,b20,b21;
  string time;
  string line;
  float returning;
  std::ifstream timestream("/proc/" + std::to_string(pid_) + "stat");
  if(timestream.is_open()){
    std::getline(timestream, line);
    std::istringstream stime(line);
    stime >> b1 >> b2 >> b3 >> b4 >> b5 >> b6 >> b7 >> b8 >> b9 >> b10 >> b11 >> b12 >> b13 >> u >> s >> cu >> cs >> b18 >> b19 >> b20 >> b21 >> time;
    float times = std::stol(time) / sysconf(_SC_CLK_TCK);
    time = Format::ElapsedTime(times);
    float sutime = stoi(u)+stoi(s)+stoi(cu)+stoi(cs);
    sutime/=sysconf(_SC_CLK_TCK);
    float seconds = UpTime()-times;
    returning = (sutime/seconds);
  }
  return returning;
}

// TODO: Return the command that generated this process
string Process::Command() {
   
  if(LinuxParser::Command(pid_).length()>40){
    string s;
    for(int i = 0; i<40;i++){
      s.push_back(LinuxParser::Command(pid_)[i]);
    }
    s.push_back('.');
    s.push_back('.');
    s.push_back('.');
    return s;
  }
  else{
    return LinuxParser::Command(pid_); 
  }
}

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const {return LinuxParser::UpTime(pid_);}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return a.CpuUtilization()<this->CpuUtilization(); }

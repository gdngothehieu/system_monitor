#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
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

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
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



// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13;
  string u, s, cu, cs;
  string line;
  int returning = 0;
  std::ifstream pidstream("/proc/" + std::to_string(pid) + "stat");
  if(pidstream.is_open()){
    std::getline(pidstream, line);
    std::istringstream spid(line);
    spid >> b1 >> b2 >> b3 >> b4 >> b5 >> b6 >> b7 >> b8 >> b9 >> b10 >> b11 >> b12 >> b13 >> u >> s >> cu >> cs;
    returning = std::stoi(u)+ std::stoi(s) + std::stoi(cu) + std::stoi(cs);
  }
  return returning;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string key;
  string line;
  string use, nic, idl, syste, iowai, ir, softIr, stea;
  string file = "/proc/stat";
  std::ifstream cpustream(file);
  if(cpustream.is_open()){
    while(std::getline(cpustream, line)){
      std::istringstream stem(line);
      while(stem >> key >> use >> nic >> idl >> syste >> iowai >> ir >> softIr >> stea){
        if(key == "cpu"){
          float user, nice, system, irq, softIrq, steal;
          user = stof(use);
          nice = stof(nic);
          system = stof(syste);
          irq = stof(ir);
          softIrq = stof(softIr);
          steal = stof(stea);
          float act = user + nice + system + irq + softIrq + steal;
          return act;
        }
      }
    }
  }
  return 0.0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string key;
  string line;
  string use, nic, idl, syste, iowai;
  string file = "/proc/stat";
  std::ifstream cpustream(file);
  if(cpustream.is_open()){
    while(std::getline(cpustream, line)){
      std::istringstream stem(line);
      while(stem >> key >> use >> nic >> idl >> syste >> iowai){
        if(key == "cpu"){
          float idle, iowait;
          idle = stof(idl);
          iowait = stof(iowai);
          float Idle = iowait + idle;
          return Idle;
        }
      }
    }
  }
  return 0.0;
}
// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies()+IdleJiffies(); }
// TODO: Read and return CPU utilization



std::vector<std::string> LinuxParser::CpuUtilization() {
  string b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,u,s,cu,cs,b18,b19,b20,b21;
  string time;
  string line;
  std::vector<std::string> returning;
  vector<int> pids = Pids();
  int size = pids.size();
  for(int i = 0;i < size;i++){
    int pid=pids[i];
    std::ifstream timestream("/proc/" + std::to_string(pid) + "stat");
    if(timestream.is_open()){
      std::getline(timestream, line);
      std::istringstream stime(line);
      stime >> b1 >> b2 >> b3 >> b4 >> b5 >> b6 >> b7 >> b8 >> b9 >> b10 >> b11 >> b12 >> b13 >> u >> s >> cu >> cs >> b18 >> b19 >> b20 >> b21 >> time;
      float times = std::stol(time) / sysconf(_SC_CLK_TCK);
      time = Format::ElapsedTime(times);
      float sutime = stoi(u)+stoi(s)+stoi(cu)+stoi(cs);
      sutime/=sysconf(_SC_CLK_TCK);
      float seconds = UpTime()-times;
      float pushing = 100*(sutime/seconds);
      returning.push_back(std::to_string(pushing));
    }
  }
  return returning;
}




// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
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

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
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

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command;
  string line;
  string from = "/proc/" + std::to_string(pid) + "/cmdline";
  std::ifstream comstream(from);
  if(comstream.is_open()){
    std::getline(comstream, line);
    std::istringstream strem(line);
    strem >> command;
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream uidstream("/proc/" + std::to_string(pid) + "/status");
  if (uidstream.is_open()) {
    while (std::getline(uidstream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmRSS:") { // using VmRSS instead of VmSize
          int divide = std::stoi(value)/1024; // to be in mB
          return to_string(divide);
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream uidstream("/proc/" + std::to_string(pid) + "/status");
  if (uidstream.is_open()) {
    while (std::getline(uidstream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string key, value, x, line;
  string uid = Uid(pid);
  std::ifstream namestream("/etc/passwd");
  if(namestream.is_open()){
    while(std::getline(namestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream strem(line);
      strem >> value >> x >> key;
      if(key == uid){
        return value;
      }
    }
  }
  return value;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function





long LinuxParser::UpTime(int pid) {
  string b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21;
  string time;
  string line;
  std::ifstream timestream("/proc/" + std::to_string(pid) + "stat");
  if(timestream.is_open()){
    std::getline(timestream, line);
    std::istringstream stime(line);
    stime >> b1 >> b2 >> b3 >> b4 >> b5 >> b6 >> b7 >> b8 >> b9 >> b10 >> b11 >> b12 >> b13 >> b14 >> b15 >> b16 >> b17 >> b18 >> b19 >> b20 >> b21 >> time;
    int times = std::stol(time)/ sysconf(_SC_CLK_TCK);
    time = Format::ElapsedTime(times);
    return times;
  }
  return 0;
}






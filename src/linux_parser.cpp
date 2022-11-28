#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value = "Linux";
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

template <typename T>
T findValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;

  std::ifstream stream(filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};

template <typename T>
T getValueOfFile(std::string const &filename) {
  std::string line;
  T value;

  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};

float LinuxParser::MemoryUtilization() { 
  string memFilename = kProcDirectory +  kMeminfoFilename;
  float totalUtilization = findValueByKey<float>(filterMemTotalString, memFilename);
  float freeResources = findValueByKey<float>(filterMemFreeString, memFilename);
  
  return (totalUtilization-freeResources)/totalUtilization; 
}


long LinuxParser::UpTime() { 
  string filename = kProcDirectory + kUptimeFilename;
  long upTime = getValueOfFile<long>(filename);
  return upTime;
}

vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpus;
  string line, key, value;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    while(std::getline(fileStream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
          if(key.find(filterCpu))
              cpus.emplace_back(value);
      }
    }
  }
  return cpus;
}

float LinuxParser::CpuUtilization(int pid) { 
  float upTime = (float)LinuxParser::UpTime();
  float hertz = (float)sysconf(_SC_CLK_TCK);
  float totalTime, seconds, cpu_usage;
  string line, key, value;
  vector<string> processorStats;
  
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      processorStats.emplace_back(value);
    }
  }
  
  totalTime = stof(processorStats[CPUStates::utime]) + stof(processorStats[CPUStates::stime]) + stof(processorStats[CPUStates::cutime]) + stof(processorStats[CPUStates::cstime]);
  seconds = upTime - (stof(processorStats[CPUStates::starttime]) / hertz);
  cpu_usage = ((totalTime / hertz)/seconds);
  return cpu_usage;
}

int LinuxParser::TotalProcesses() { 
  int totalProcesses;
  string line, key, value;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    while(std::getline(fileStream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
          if(key == filterProcesses)
              totalProcesses = stoi(value);
      }
    }
  }
  return totalProcesses;
}

int LinuxParser::RunningProcesses() { 
  string filename = kProcDirectory + kStatFilename;
  int runningProcesses = findValueByKey<int>(filterRunningProcesses, filename);
  return runningProcesses;
}

string LinuxParser::Command(int pid) {
  string filename = kProcDirectory + to_string(pid) + kCmdlineFilename;
  return string(getValueOfFile<string>(filename));
  
}

string LinuxParser::Ram(int pid) { 
  string filename = kProcDirectory + to_string(pid) + kStatusFilename;
  float ram = findValueByKey<float>(filterProcMem, filename);
  std::ostringstream out;
  out.precision(2);
  out << std::fixed << ram / 1024; // convert from kb to mb and save to stream
  return out.str(); 
}

string LinuxParser::Uid(int pid) { 
  string filename = kProcDirectory + to_string(pid) + kStatFilename;
  string uid = findValueByKey<string>(filterUID, filename);
  return uid;
}

string LinuxParser::User(int pid) { 
  string line, user, letter, value;
  string username = "";
  string uid = LinuxParser::Uid(pid);
  std::ifstream fileStream(kPasswordPath);
  if (fileStream.is_open()) {
    while(std::getline(fileStream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> letter >> value;
      if(value == uid)
        username = user;
    }
  }
  return username; 
}

long LinuxParser::UpTime(int pid) { 
  string line, value;
  long upTimeValue = 0;
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (fileStream.is_open()) {
    while(std::getline(fileStream, line)){
      std::istringstream linestream(line);
      for(int i = 0; i <22; i++){
          linestream >> value;
      }
      upTimeValue = stol(value);
    }
  }
  return LinuxParser::UpTime() - upTimeValue/sysconf(_SC_CLK_TCK);
}

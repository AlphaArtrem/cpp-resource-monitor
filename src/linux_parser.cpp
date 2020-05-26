#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read operating system data from the filesystem
string LinuxParser::OperatingSystem()
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "PRETTY_NAME")
        {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read kernel data from the filesystem
string LinuxParser::Kernel()
{
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Get process ids
vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr)
  {
    // Is this a directory?
    if (file->d_type == DT_DIR)
    {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit))
      {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  std::string line, name, size, unit;
  vector<float> mem;
  if (stream.is_open())
  {
    for (int i = 0; i < 3; i++)
    {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> name >> size >> unit;
      mem.push_back(std::stof(size));
    }
  }

  return ((mem[0] - mem[2]) / mem[0]);
}

// Read and return the system uptime
long LinuxParser::UpTime()
{
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  std::string line, uptime;
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return std::stol(uptime);
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string line, value;
  int processes = 0;

  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream stringstream(line);
      stringstream >> value;
      if(value == "processes"){
        stringstream >> value;
        processes = std::stoi(value);
        break;
      }      
    }
  }

  return processes;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string line, value;
  int processes = 0;

  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream stringstream(line);
      stringstream >> value;
      if(value == "procs_running"){
        stringstream >> value;
        processes = std::stoi(value);
        break;
      }      
    }
  }

  return processes;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::string line;
  std::getline(stream, line);
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid [[maybe_unused]]) {
  return string();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string line, value;
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream stringstream(line);
      stringstream >> value;
      if(value == "Uid:"){
        stringstream >> value;
        break;
      }
    }
  }
  return value;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::string line, value, uid, user, uidCopy;
  uid = Uid(pid);
  std::ifstream streamPwd(kPasswordPath);
  if(streamPwd.is_open()){
    while(std::getline(streamPwd, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stringstream(line);
      stringstream >> user >> value >> uidCopy;

      if(uid == uidCopy){
        break;
      }
    }
  }
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::string line, value;
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream stringstream(line);
    for(int i = 0; i < 22; i++){
      stringstream >> value;
    }
  }
  return sysconf(std::stol(value));
}
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return 0; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// Return the command that generated this process
string Process::Command() { return string(); }

// Return this process's memory utilization
string Process::Ram() { return string(); }

// Return the user (name) that generated this process
string Process::User() { return string(); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
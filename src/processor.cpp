#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<double> values;

    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    std::string line, value;
    if(stream.is_open())
    {
        std::getline(stream, line);
    }

    std::istringstream stringstream(line);
    stringstream >> value;
    if(value == "cpu"){
        while(stringstream >> value){
            values.push_back(std::stod(value));
        }
    }
    
    // Getting values from read stream
    const double user = values[0];
    const double nice = values[1];
    const double system = values[2];
    const double idle = values[3];
    const double iowait = values[4];
    const double irq = values[5];
    const double softirq = values[6];
    const double steal = values[7];

    // Calculating Cpu usage precantage
    const double PrevIdle = previdle + previowait;
    const double Idle = idle + iowait;
    const double PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    const double NonIdle = user + nice + system + irq + softirq + steal;
    const double PrevTotal = PrevIdle + PrevNonIdle;
    const double Total = Idle + NonIdle;

    const double totald = Total - PrevTotal;
    const double idled = Idle - PrevIdle;
    const double CpuUsage = (totald - idled) / totald;

    // Updating previous values
    prevuser = values[0];
    prevnice = values[1];
    prevsystem = values[2];
    previdle = values[3];
    previowait = values[4];
    previrq = values[5];
    prevsoftirq = values[6];
    prevsteal = values[7];
        
    return CpuUsage; 
}
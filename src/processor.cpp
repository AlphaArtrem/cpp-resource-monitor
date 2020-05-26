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
    double user = values[0];
    double nice = values[1];
    double system = values[2];
    double idle = values[3];
    double iowait = values[4];
    double irq = values[5];
    double softirq = values[6];
    double steal = values[7];

    // Calculating Cpu usage precantage
    double PrevIdle = previdle + previowait;
    double Idle = idle + iowait;
    double PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    double NonIdle = user + nice + system + irq + softirq + steal;
    double PrevTotal = PrevIdle + PrevNonIdle;
    double Total = Idle + NonIdle;

    double totald = Total - PrevTotal;
    double idled = Idle - PrevIdle;
    double CpuUsage = (totald - idled) / totald;

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
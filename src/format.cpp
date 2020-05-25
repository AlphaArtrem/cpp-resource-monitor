#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    string formattedTime = std::to_string(int(seconds / (60 * 60))) + ':';
    seconds %= 60 * 60;
    formattedTime += std::to_string(int(seconds / 60)) + ':';
    seconds %= 60;
    formattedTime += std::to_string(int(seconds));
    return formattedTime;
}
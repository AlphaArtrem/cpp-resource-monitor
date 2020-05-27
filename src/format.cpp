#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {\
    string buffer = std::to_string(int(seconds / (60 * 60)));
    string formattedTime = buffer.length() == 1 ? '0' + buffer : buffer + ':';
    seconds %= 60 * 60;
    buffer = std::to_string(int(seconds / 60));
    formattedTime += buffer.length() == 1 ? '0' + buffer : buffer + ':';
    seconds %= 60;
    buffer = std::to_string(int(seconds));
    formattedTime += buffer.length() == 1 ? '0' + buffer : buffer;
    return formattedTime;
}
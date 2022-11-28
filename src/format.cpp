#include <string>
#include <iomanip>
#include <chrono>
#include "format.h"

using std::string;
using std::setw;
using std::setfill;
using std::stringstream;

string Format::ElapsedTime(long sTime) {   
  std::chrono::seconds seconds{sTime};
  std::chrono::hours hours =
      std::chrono::duration_cast<std::chrono::hours>(seconds);
  seconds -= std::chrono::duration_cast<std::chrono::seconds>(hours);

  std::chrono::minutes minutes =
      std::chrono::duration_cast<std::chrono::minutes>(seconds);
  seconds -= std::chrono::duration_cast<std::chrono::seconds>(minutes);
  
  stringstream timeStream{};
  timeStream << setw(2) << setfill('0') << hours.count()
    << setw(1) << ":" 
    << setw(2) << setfill('0') << minutes.count()
    << setw(1) << ":" 
    << setw(2) << setfill('0') << seconds.count();
  return timeStream.str();
}
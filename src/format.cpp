#include <string>
#include <cmath>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long int minutes = floor(seconds/60);
  string z = "0";
  string second;
  string minute;
  if((seconds%60)<=9){
    second = "0" + std::to_string(seconds%60);
  }else{
    second = std::to_string(seconds%60);
  }
  if((minutes%60) <= 9){
    minute = "0" + std::to_string(minutes%60);
  } else{
    minute = std::to_string(minutes%60);
  }
  int hours =  floor(minutes/60);
  string answer = std::to_string(hours) + ":" + minute + ":" + second;
  return answer;
}

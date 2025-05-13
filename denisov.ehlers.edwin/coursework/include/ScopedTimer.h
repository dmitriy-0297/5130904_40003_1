#ifndef SCOPEDTIMER_H
#define SCOPEDTIMER_H
#include <chrono>
#include <iostream>
#include <string>


class ScopedTimer
{
public:
  ScopedTimer(const std::string& label);

  void suppress();

  ~ScopedTimer();

private:
  std::string label_;
  std::chrono::high_resolution_clock::time_point start_;
  bool isSuppressed = false;
};


#endif //SCOPEDTIMER_H

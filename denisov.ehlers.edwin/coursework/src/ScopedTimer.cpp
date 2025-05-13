#include "../include/ScopedTimer.h"

using namespace std::chrono;

ScopedTimer::ScopedTimer(const std::string& label) :
  label_(label), start_(high_resolution_clock::now())
{
}

void ScopedTimer::suppress()
{
  isSuppressed = true;
}

ScopedTimer::~ScopedTimer()
{
  const auto end = high_resolution_clock::now();
  const auto duration = duration_cast<milliseconds>
    (end - start_).count();
  if (!isSuppressed)
    std::cout << "[" << label_ << "] Time: " << duration << " ms\n";
}

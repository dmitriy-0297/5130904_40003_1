#ifndef STREAMGUARD_H
#define STREAMGUARD_H
#include <ios>


class StreamGuard
{
public:
  explicit StreamGuard(std::basic_ios<char>& s);
  ~StreamGuard();

private:
  std::basic_ios<char>& s_;
  std::streamsize width_;
  std::streamsize precision_;
  std::basic_ios<char>::fmtflags fmt_;
};


#endif //STREAMGUARD_H

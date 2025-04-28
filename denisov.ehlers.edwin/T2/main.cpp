#include <iostream>
#include <iterator>
#include <vector>
#include <regex>

#include "DataStruct.h"


int main()
{
  std::vector<DataStruct> dataVector;
  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator<DataStruct>(std::cin),
      std::istream_iterator<DataStruct>(),
      std::back_inserter(dataVector)
    );
  }

  std::sort(
    dataVector.begin(),
    dataVector.end(),
    compareDataStructs
  );

  const std::ostream_iterator<DataStruct> out(std::cout, "\n");
  std::copy(
    dataVector.begin(),
    dataVector.end(),
    out
  );

  return 0;
}

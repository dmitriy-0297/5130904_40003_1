#include <iostream>
#include <vector>
#include "DataStruct.h"
#include <algorithm>
#include <iterator>
using namespace mydatastruct;

int main()
{
  try
  {
    std::vector<DataStruct> v;
    fillvector(v);

    std::sort(v.begin(), v.end(), datastructSort);

    std::copy(v.begin(),v.end(), std::ostream_iterator<DataStruct>(std::cout ,"\n"));

  }
  catch (std::invalid_argument &e)
  {
    std::cout << e.what();
    return 1;
  }
  return 0;

}

#ifndef BYTEENTRY_H
#define BYTEENTRY_H
#include <cstdint>
#include <ostream>
#include <vector>

#include "types.h"


struct ByteEntry
{
  ByteEntry();

  ByteEntry(uint8_t byte, int occurrences);

  friend std::ostream& operator<<(std::ostream& os, const ByteEntry& obj);

  uint8_t byte;
  int occurrences;
  Encoded code;
};


#endif //BYTEENTRY_H

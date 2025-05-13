#ifndef BITUTILS_H
#define BITUTILS_H
#include <cstdint>
#include <vector>

#include "FanoExceptions.h"
#include "types.h"


namespace bit_utils
{
  constexpr size_t DEFAULT_LINE_WIDTH = 100;

  static constexpr int BITS_IN_BYTE = 8;

  Encoded byteToBits(uint8_t byte);

  uint8_t bitsToByte(const Encoded& bits);

  Packed packBits(const Encoded& bits);

  Encoded processCodeBuffer(const Encoded& buffer);

  Encoded addLeadingZerosToCode(const Encoded& code, size_t length);

  Encoded normalizeCode(const Encoded& code, size_t length);

  Encoded byteBufferToBits(const std::vector<uint8_t>& buffer);
}


#endif //BITUTILS_H

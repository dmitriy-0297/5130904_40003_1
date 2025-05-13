#include "../include/bit_utils.h"

#include <iostream>


Encoded bit_utils::byteToBits(const uint8_t byte)
{
  Encoded bits;
  bits.reserve(BITS_IN_BYTE);
  for (int i = 0; i < BITS_IN_BYTE; i++)
  {
    const bool bit = byte >> i & 1;
    bits += bit ? '1' : '0';
  }
  return bits;
}

uint8_t bit_utils::bitsToByte(const Encoded& bits)
{
  if (bits.size() > BITS_IN_BYTE)
    throw FanoException("bitsToByte: size exceeds 8 bits");

  uint8_t result = 0;
  for (size_t i = 0; i < bits.size(); i++)
    if (bits[i] == '1')
      result |= static_cast<uint8_t>(1 << i);

  return result;
}

Packed bit_utils::packBits(const Encoded& bits)
{
  Packed bytes((bits.size() + 7) / BITS_IN_BYTE, 0);

  size_t bitIndex = 0;
  for (auto const& bit : bits)
  {
    if (bit == '1')
    {
      const size_t byteIndex = bitIndex >> 3;
      const uint8_t mask = static_cast<uint8_t>(1) << (bitIndex & 7);
      bytes[byteIndex] |= mask;
    }
    bitIndex++;
  }

  return bytes;
}

Encoded bit_utils::processCodeBuffer(const Encoded& buffer)
{
  constexpr int NOT_FOUND = -1;
  int startIndex = NOT_FOUND;
  for (int i = 0; i < static_cast<int>(buffer.size()); i++)
    if (buffer[i] == '1')
    {
      startIndex = i + 1;
      break;
    }

  if (startIndex == NOT_FOUND || startIndex >= static_cast<int>(buffer.size()))
    throw FanoException("Incorrect code format");

  Encoded processedBuffer(buffer.begin() + startIndex, buffer.end());
  return processedBuffer;
}

Encoded bit_utils::addLeadingZerosToCode(const Encoded& code,
                                              const size_t length)
{
  Encoded result(length, '0');
  for (size_t i = 1; i <= code.size(); i++)
    result[length - i] = code[code.size() - i];

  return result;
}

Encoded bit_utils::normalizeCode(const Encoded& code,
                                      const size_t length)
{
  if (code.size() > length)
    throw FanoException(
      "normalizeCode: code is longer than expected length"
      );

  Encoded withZeros = addLeadingZerosToCode(code, length + 1);
  withZeros[length - code.size()] = '1';
  return withZeros;
}

Encoded bit_utils::byteBufferToBits(const std::vector<uint8_t>& buffer)
{
  Encoded bits;
  bits.reserve(buffer.size() * BITS_IN_BYTE);

  for (const uint8_t byte : buffer)
    for (int i = 0; i < BITS_IN_BYTE; i++)
      bits += (byte >> i & 1) ? '1' : '0';

  return bits;
}

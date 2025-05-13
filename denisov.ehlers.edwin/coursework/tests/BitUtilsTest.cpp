#include "../include/bit_utils.h"
#include <cassert>
#include <iostream>

namespace BitUtilsTests
{

  void testByteToBitsAndBack()
  {
    uint8_t original = 0b10101010;
    Encoded bits = bit_utils::byteToBits(original);
    assert(bits.size() == 8);
    uint8_t result = bit_utils::bitsToByte(bits);
    assert(result == original);
  }

  void testBitsToByteThrows()
  {
    Encoded tooLong = "111111111"; // 9 bits
    bool caught = false;
    try
    {
      bit_utils::bitsToByte(tooLong);
    }
    catch (const FanoException &)
    {
      caught = true;
    }
    assert(caught);
  }

  void testPackBits()
  {
    Encoded bits = "01010101";
    // equals to 10101010 because of the little-endian ordering
    Packed packed = bit_utils::packBits(bits);
    assert(packed.size() == 1);
    assert(packed[0] == 0b10101010);
  }

  void testProcessCodeBuffer()
  {
    Encoded input = "00011101"; // 3 leading zeros, 1, then 3 bits
    Encoded processed = bit_utils::processCodeBuffer(input);
    assert(processed == "1101");
  }

  void testAddLeadingZerosToCode()
  {
    Encoded code = "101";
    Encoded padded = bit_utils::addLeadingZerosToCode(code, 6);
    assert(padded == "000101");
  }

  void testNormalizeCode()
  {
    Encoded code = "101";
    Encoded normalized = bit_utils::normalizeCode(code, 6);
    assert(normalized == "0001101");
    // one '1' before code, rest are leading zeros
  }

  void testByteBufferToBits()
  {
    std::vector<uint8_t> buffer = {0b00001111};
    Encoded bits = bit_utils::byteBufferToBits(buffer);
    assert(bits.size() == 8);
    for (int i = 0; i < 4; i++)
      assert(bits[i] == '1');
    for (int i = 4; i < 8; i++)
      assert(bits[i] == '0');
  }

  void runBitUtilsTest()
  {
    std::cout << "[BitUtilsTest] Running...\n";
    testByteToBitsAndBack();
    testBitsToByteThrows();
    testPackBits();
    testProcessCodeBuffer();
    testAddLeadingZerosToCode();
    testNormalizeCode();
    testByteBufferToBits();
    std::cout << "[BitUtilsTest] All tests passed\n";
  }

}

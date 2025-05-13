#include "../include/Table.h"
#include <cassert>
#include <iostream>

namespace TableTests {

  void testTableBuildAndEncodeDecode() {
    Buffer buffer = { 'A', 'B', 'A', 'C', 'A' };

    Table table(buffer);
    Encoded encoded = table.encode();
    uint8_t bitsPerCode = bit_utils::bitsToByte(
        Encoded(encoded.begin() + 8, encoded.begin() + 16));
    Encoded cleaned(encoded.begin() + 16, encoded.end());
    Table restored;
    restored.decode(cleaned, bitsPerCode);

    for (const auto& pair : table.getRawTable()) {
      const Encoded& code = pair.second.code;
      uint8_t decodedByte = 0;

      bool success = restored.getByteByCode(code, decodedByte);
      assert(success);
      assert(decodedByte == pair.first);
    }
  }

  void testTableThrowsOnEmptyBuffer() {
    bool caught = false;
    try {
      Buffer buffer;
      Table table(buffer);
    } catch (const FileException&) {
      caught = true;
    }
    assert(caught);
  }

  void runTableTest() {
    std::cout << "[TableTest] Running...\n";
    testTableBuildAndEncodeDecode();
    testTableThrowsOnEmptyBuffer();
    std::cout << "[TableTest] All tests passed\n";
  }

}

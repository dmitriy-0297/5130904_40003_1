#include "../include/Decoder.h"

Decoder::Decoder() = default;

Decoder::Decoder(const Decoder&) = default;

Decoder::Decoder(Decoder&&) noexcept = default;

Decoder& Decoder::operator=(const Decoder&) = default;

Decoder& Decoder::operator=(Decoder&&) noexcept = default;

Decoder::~Decoder() = default;

void Decoder::decode(const std::string& inputFilePath,
                     const std::string& outputFilePath)
{
  /*
      ===== BINARY FILE DATA STORAGE SCHEME =====

      [0]      [1]            [2]
      ┌──────┬──────────────┬────────────┐
      │  1B  │     1B       │    1B      │
      │unused│ numOfEntries │ bitsPerCode│
      └──────┴──────────────┴────────────┘
           │        │             │
           ▼        ▼             ▼
       Number   Number of     Length of
       of       unique        code (with
       unused   symbols       leading 1
       bits     in table      included)

      Then comes the encoding table block:
      Repeated numOfEntries times:
          ┌────────────┬─────────────────────────────┐
          │   1 byte   │ bitsPerCode bits            │
          │  symbol    │ normalized code             │
          └────────────┴─────────────────────────────┘

          normalized code = [0 ... 0][1][actual_code]
                            ^ padding ^

      After the table: encoded data block
          ┌──────────────────────────────────────────────┐
          │ encoded data (bit vector)                    │
          └──────────────────────────────────────────────┘
          (trailing unused bits are ignored)

      All data after the first 3 bytes is treated as a bit stream.
      */

  ScopedTimer scopedTimer("Decoder");
  try
  {
    file_io::checkFiles(inputFilePath, outputFilePath);
    const Buffer rawBuffer = file_io::readFileToBuffer(inputFilePath);
    constexpr size_t HEADER_SIZE = 3;
    constexpr size_t INDEX_UNUSED_BITS_QUANTITY = 0;
    constexpr size_t INDEX_NUM_OF_ENTRIES = 1;
    constexpr size_t INDEX_BITS_PER_CODE = 2;

    if (rawBuffer.size() < HEADER_SIZE)
      throw DecoderException("Incorrect header format");

    const uint8_t unusedBitsQuantity = rawBuffer[INDEX_UNUSED_BITS_QUANTITY];
    const uint8_t numberOfEntries = rawBuffer[INDEX_NUM_OF_ENTRIES];
    const uint8_t bitsPerCode = rawBuffer[INDEX_BITS_PER_CODE];

    const Buffer cleanedBuffer(rawBuffer.begin() + 3, rawBuffer.end());
    const Encoded encodedBuffer = bit_utils::byteBufferToBits(cleanedBuffer);

    const size_t tableBitSize = getTableBitSize(numberOfEntries, bitsPerCode);
    const Encoded encodedTable(
      encodedBuffer.begin(),
      encodedBuffer.begin() + static_cast<long long>(tableBitSize)
    );
    const Encoded encodedData(
      encodedBuffer.begin() + static_cast<long long>(tableBitSize),
      encodedBuffer.end() - unusedBitsQuantity
    );

    Table table;
    table.decode(encodedTable, bitsPerCode);

    Data data;
    data.decode(table, encodedData);

    file_io::writeToFile(outputFilePath, data.getData());
  }
  catch (const std::exception& ex)
  {
    scopedTimer.suppress();
    std::cerr << ex.what() << "\n";
  }
}

size_t Decoder::getTableBitSize(const uint8_t numberOfEntries,
                                const uint8_t bitsPerCode)
{
  return numberOfEntries * (8 + bitsPerCode);
}

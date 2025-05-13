#include "../include/Encoder.h"

#include <iomanip>

Encoder::Encoder() = default;

Encoder::Encoder(const Encoder&) = default;

Encoder::Encoder(Encoder&&) noexcept = default;

Encoder& Encoder::operator=(const Encoder&) = default;

Encoder& Encoder::operator=(Encoder&&) noexcept = default;

Encoder::~Encoder() = default;

void Encoder::encode(const std::string& inputFilePath, const std::string& outputFilePath)
{
  ScopedTimer scopedTimer("Encoder");
  try
  {
    file_io::checkFiles(inputFilePath, outputFilePath);
    const Buffer buffer = file_io::readFileToBuffer(inputFilePath);
    Table table(buffer);
    Data data(buffer);

    const Encoded encodedTable = table.encode();
    const Encoded encodedData = data.encode(table);

    const Packed packed = packEncodedTableAndData(encodedTable, encodedData);

    file_io::writeToFile(outputFilePath, packed);

    getStatistics(inputFilePath, outputFilePath, table);
  }
  catch (const std::exception& ex)
  {
    scopedTimer.suppress();
    std::cerr << ex.what() << "\n";
  }
}

Packed Encoder::packEncodedTableAndData(const Encoded& encodedTable,
                                        const Encoded& encodedData)
{
  Encoded encodedAll;

  const uint8_t unusedBitsQuantity = (
    bit_utils::BITS_IN_BYTE - (encodedTable.size() + encodedData.size())
    % bit_utils::BITS_IN_BYTE
  ) % bit_utils::BITS_IN_BYTE;
  const auto& encodedQuantity = bit_utils::byteToBits(unusedBitsQuantity);
  encodedAll += encodedQuantity;
  encodedAll += encodedTable;
  encodedAll += encodedData;

  return bit_utils::packBits(encodedAll);
}

void Encoder::getStatistics(const std::string& inputFilePath,
                            const std::string& outputFilePath,
                            const Table& table)
{
  const size_t inputSize = file_io::getFileSize(inputFilePath);
  const size_t outputSize = file_io::getFileSize(outputFilePath);
  std::cout << "[Encoder] Input size: " << inputSize << " bytes\n";
  std::cout << "[Encoder] Encoded size: " << outputSize << " bytes\n";

  const double compressionRatio = (
    1 - static_cast<double>(outputSize) / static_cast<double>(inputSize)
  ) * 100;
  std::cout << "[Encoder] Compression ratio: " << std::fixed <<
    std::setprecision(2) << compressionRatio << "%\n";

  const double entropy = table.calculateEntropy();
  std::cout << "[Encoder] Entropy: " << entropy << "\n";
}

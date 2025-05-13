#ifndef TABLE_H
#define TABLE_H
#include <fstream>
#include <unordered_map>

#include "bit_utils.h"
#include "ByteEntry.h"
#include "types.h"
#include "FanoExceptions.h"

class Table
{
public:
  explicit Table(const Buffer& buffer);

  Table();

  Table(const Table&);

  Table(Table&&) noexcept;

  Table& operator=(const Table&);

  Table& operator=(Table&&) noexcept;

  ~Table();

  Encoded encode();

  void decode(const Encoded& encodedTable, uint8_t bitsPerCode);

  double calculateEntropy() const;

  const Encoded& getCodeForByte(uint8_t byte) const;

  bool getByteByCode(const Encoded& code, uint8_t& outByte) const;

  const std::unordered_map<uint8_t, ByteEntry>& getRawTable() const;

private:
  void countByteFrequencies(const Buffer& buffer);

  std::vector<ByteEntry> toVector() const;

  static void sortTableVectorByFrequency(std::vector<ByteEntry>& tableVector);

  static void buildFanoCodes(std::vector<ByteEntry>& tableVector, size_t start,
                             size_t end);

  void fromVector(std::vector<ByteEntry>& tableVector);

  size_t findMaxCodeLength() const;

  void buildReverseTable();

  std::unordered_map<uint8_t, ByteEntry> table_;
  std::unordered_map<Encoded, uint8_t> reverseTable_;
};


#endif //TABLE_H

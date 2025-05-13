#include "../include/Table.h"

#include <algorithm>
#include <cmath>
#include <iostream>


Table::Table(const Buffer& buffer)
{
  if (buffer.empty())
    throw FileException("File is empty");

  countByteFrequencies(buffer);
  std::vector<ByteEntry> tableVector = toVector();
  sortTableVectorByFrequency(tableVector);
  buildFanoCodes(tableVector, 0, tableVector.size());
  fromVector(tableVector);
}

Table::Table() = default;

Table::Table(const Table&) = default;

Table::Table(Table&&) noexcept = default;

Table& Table::operator=(const Table&) = default;

Table& Table::operator=(Table&&) noexcept = default;

Table::~Table() = default;

Encoded Table::encode()
{
  const size_t bitsPerCode = findMaxCodeLength();
  if (bitsPerCode > 255)
    throw TableException("Code is too long");

  Encoded encodedTable;
  encodedTable += bit_utils::addLeadingZerosToCode(
    bit_utils::byteToBits(table_.size()), bit_utils::BITS_IN_BYTE);

  encodedTable += bit_utils::addLeadingZerosToCode(
    bit_utils::byteToBits(bitsPerCode + 1), bit_utils::BITS_IN_BYTE);

  for (const auto& pair : table_)
  {
    encodedTable += bit_utils::byteToBits(pair.first);
    encodedTable += bit_utils::normalizeCode(pair.second.code, bitsPerCode);
  }

  return encodedTable;
}

void Table::decode(const Encoded& encodedTable, const uint8_t bitsPerCode)
{
  table_.clear();
  reverseTable_.clear();
  const size_t recordSize = bit_utils::BITS_IN_BYTE + bitsPerCode;

  for (size_t i = 0; i + recordSize <= encodedTable.size(); i += recordSize)
  {
    const auto offset = static_cast<long long>(i);
    Encoded symbolBits(encodedTable.begin() + offset,
                            encodedTable.begin() + offset +
                            bit_utils::BITS_IN_BYTE);
    Encoded codeBits(encodedTable.begin() + offset +
                          bit_utils::BITS_IN_BYTE,
                          encodedTable.begin() + offset +
                          static_cast<long long>(recordSize));

    ByteEntry byteEntry;
    byteEntry.byte = bit_utils::bitsToByte(symbolBits);
    byteEntry.code = bit_utils::processCodeBuffer(codeBits);
    table_[byteEntry.byte] = byteEntry;
  }

  buildReverseTable();
}

double Table::calculateEntropy() const
{
  int totalQuantity = 0;
  double entropy = 0.0f;
  for (const auto& pair : table_)
  {
    totalQuantity += pair.second.occurrences;
  }
  for (const auto& pair : table_)
  {
    const double freq = static_cast<double>(pair.second.occurrences) /
      totalQuantity;
    entropy += freq * std::log2(freq);
  }
  return -1 * entropy;
}

const Encoded& Table::getCodeForByte(const uint8_t byte) const
{
  static const Encoded EMPTY_ENCODED;
  const auto it = table_.find(byte);
  if (it != table_.end()) {
    return it->second.code;
  }
  return EMPTY_ENCODED;
}

bool Table::getByteByCode(const Encoded& code, uint8_t& outByte) const
{
  const auto search = reverseTable_.find(code);
  if (search == reverseTable_.end())
    return false;

  outByte = search->second;
  return true;
}

const std::unordered_map<uint8_t, ByteEntry>& Table::getRawTable() const
{
  return table_;
}

void Table::countByteFrequencies(const Buffer& buffer)
{
  for (const uint8_t byte : buffer)
  {
    auto& entry = table_[byte];
    entry.occurrences++;
    entry.byte = byte;
  }
}

std::vector<ByteEntry> Table::toVector() const
{
  std::vector<ByteEntry> tableVector;
  tableVector.reserve(table_.size());
for (const auto& pair : table_)
    tableVector.push_back(pair.second);
  return tableVector;
}


void Table::sortTableVectorByFrequency(std::vector<ByteEntry>& tableVector)
{
  std::sort(tableVector.begin(), tableVector.end(),
            [](const ByteEntry& a, const ByteEntry& b) {
              return a.occurrences > b.occurrences;
            });
}

void Table::buildFanoCodes(std::vector<ByteEntry>& tableVector,
                           const size_t start, const size_t end)
{
  if (end - start == 1)
  {
    tableVector[start].code += '0';
    return;
  }

  size_t total = 0;
  for (size_t i = start; i < end; i++)
    total += tableVector[i].occurrences;

  size_t split = start;
  long long sum = tableVector[split].occurrences;

  while (split < end && sum * 2 <= total)
  {
    split++;
    sum += tableVector[split].occurrences;
  }
  if (split == start) ++split;
  if (split == end) --split;

  for (size_t i = start; i < split; i++)
    tableVector[i].code += '0';

  for (size_t i = split; i < end; i++)
    tableVector[i].code += '1';

  buildFanoCodes(tableVector, start, split);
  buildFanoCodes(tableVector, split, end);
}

void Table::fromVector(std::vector<ByteEntry>& tableVector)
{
  for (const auto& byteEntry : tableVector)
    table_[byteEntry.byte] = byteEntry;
}

size_t Table::findMaxCodeLength() const
{
  if (table_.empty())
    throw TableException("Table is empty");

  size_t max = 0;

  for (const auto& pair : table_)
    max = std::max(max, pair.second.code.size());

  return max;
}

void Table::buildReverseTable()
{
  for (const auto& pair : table_)
    reverseTable_[pair.second.code] = pair.first;
}

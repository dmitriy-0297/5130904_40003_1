#ifndef DATA_H
#define DATA_H
#include <cstdint>

#include "Table.h"


class Data
{
public:
  explicit Data(const Buffer& buffer);

  Data();

  Data(const Data&);

  Data(Data&&) noexcept;

  Data& operator=(const Data&);

  Data& operator=(Data&&) noexcept;

  ~Data();

  Encoded encode(const Table& table);

  void decode(const Table& table, const Encoded& encodedData);

  const std::vector<uint8_t>& getData() const;

private:
  std::vector<uint8_t> data_;
};


#endif //DATA_H

#ifndef DECODER_H
#define DECODER_H
#include <cstdint>
#include <fstream>
#include <utility>

#include "bit_utils.h"
#include "Data.h"
#include "file_io.h"
#include "Table.h"
#include "ScopedTimer.h"
#include "FanoExceptions.h"

class Decoder
{
public:
  Decoder();

  Decoder(const Decoder&);

  Decoder(Decoder&&) noexcept;

  Decoder& operator=(const Decoder&);

  Decoder& operator=(Decoder&&) noexcept;

  ~Decoder();

  static void decode(const std::string& inputFilePath,
                     const std::string& outputFilePath);

private:
  static size_t getTableBitSize(uint8_t numberOfEntries, uint8_t bitsPerCode);
};


#endif //DECODER_H

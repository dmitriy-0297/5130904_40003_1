#ifndef FILE_IO_H
#define FILE_IO_H

#include <fstream>

#include "types.h"
#include "FanoExceptions.h"


namespace file_io
{
  void checkFiles(const std::string& inputFilePath, const std::string& outputFilePath);

  Buffer readFileToBuffer(const std::string& inputFile);

  void writeToFile(const std::string& outputFile, const Packed& packed);

  size_t getFileSize(const std::string& filePath);
}


#endif //FILE_IO_H

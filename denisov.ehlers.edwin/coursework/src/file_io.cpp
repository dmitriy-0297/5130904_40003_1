#include "../include/file_io.h"

#include <fstream>

void file_io::checkFiles(const std::string& inputFilePath,
                         const std::string& outputFilePath)
{
  if (inputFilePath == outputFilePath)
    throw FileException("Files must be different");

  if (inputFilePath.empty() || outputFilePath.empty())
    throw FileException("File paths must not be empty");
}

Buffer file_io::readFileToBuffer(const std::string& inputFile)
{
  std::ifstream ifs(inputFile.c_str(),
                    std::ios::binary | std::ios::ate);
  if (!ifs)
    throw FileException("Cannot open the input file");

  const std::streamsize fileSize = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  if (fileSize < 0)
    throw FileException("Negative file size is invalid");

  Buffer buffer(static_cast<size_t>(fileSize), 0);
  if (!ifs.read(reinterpret_cast<char*>(buffer.data()), fileSize))
  {
    throw FileException("Failed to read file data");
  }
  // thanks to RAII, there's no need to close the stream manually
  return buffer;
}

void file_io::writeToFile(const std::string& outputFile, const Packed& packed)
{
  std::ofstream ofs(outputFile.c_str(),
                    std::ios::out | std::ios::binary | std::ofstream::trunc);
  if (!ofs)
    throw FileException("Cannot open the output file");

  ofs.write(
    reinterpret_cast<const char*>(packed.data()),
    static_cast<long long>(packed.size())
  );
  // thanks to RAII, there's no need to close the stream manually
}

size_t file_io::getFileSize(const std::string& filePath)
{
  std::ifstream file(filePath.c_str(),
                     std::ios::binary | std::ios::ate);
  return file.tellg();
}

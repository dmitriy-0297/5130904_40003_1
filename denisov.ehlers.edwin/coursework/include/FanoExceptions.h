#ifndef FANOEXCEPTIONS_H
#define FANOEXCEPTIONS_H
#include <stdexcept>


class FanoException : public std::runtime_error
{
public:
  explicit FanoException(const std::string& msg): std::runtime_error(
    "[ERROR] " + msg)
  {
  }
};

class EncoderException : public FanoException
{
public:
  explicit EncoderException(const std::string& msg): FanoException(
    "Encoder exception: " + msg)
  {
  }
};

class DecoderException : public FanoException
{
public:
  explicit DecoderException(const std::string& msg): FanoException(
    "Decoder exception: " + msg)
  {
  }
};

class FileException : public FanoException
{
public:
  explicit FileException(const std::string& msg): FanoException(
    "File exception: " + msg)
  {
  }
};

class TableException : public FanoException
{
public:
  explicit TableException(const std::string& msg): FanoException(
    "Table exception: " + msg)
  {
  }
};

class DataException : public FanoException
{
public:
  explicit DataException(const std::string& msg): FanoException(
    "Data exception: " + msg)
  {
  }
};


#endif //FANOEXCEPTIONS_H

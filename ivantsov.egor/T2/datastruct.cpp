#include "datastruct.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstdlib>

static std::string formatDoubleLit(const double value)
{
  std::ostringstream out;
  out << std::fixed << std::setprecision(1) << value;
  out << 'd';
  return out.str();
}

static bool parseDoubleLit(const std::string& text, double& value)
{
  if (text.size() < 2) {
    return false;
  }
  const char lastChar = text.back();
  if (lastChar != 'd' && lastChar != 'D') {
    return false;
  }

  const std::string numericPart = text.substr(0, text.size() - 1);

  try {
    size_t idx = 0;
    value = std::stod(numericPart, &idx);
    return idx == numericPart.length();
  }
  catch (...) {
    return false;
  }
}

static bool parseUllLit(const std::string& text, unsigned long long& value)
{
  if (text.size() < 4) {
    return false;
  }

  std::string suffix = text.substr(text.size() - 3);
  for (char& c : suffix) {
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  }
  if (suffix != "ull") {
    return false;
  }

  const std::string numericPart = text.substr(0, text.size() - 3);
  if (numericPart.empty()) {
    return false;
  }
  for (char c : numericPart) {
    if (!std::isdigit(static_cast<unsigned char>(c))) {
      return false;
    }
  }

  try {
    value = std::stoull(numericPart);
  }
  catch (...) {
    return false;
  }
  return true;
}

static void trim(std::string& s)
{
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
    s.erase(s.begin());
  }
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
    s.pop_back();
  }
}

static size_t findNextColonOutsideQuotes(const std::string& str, size_t from)
{
  bool inQuotes = false;
  for (size_t i = from; i < str.size(); ++i) {
    char ch = str[i];
    if (ch == '"') {
      inQuotes = !inQuotes;
    }
    else if (ch == ':' && !inQuotes) {
      return i;
    }
  }
  return std::string::npos;
}

static bool parseLine(const std::string& original, DataStruct_t& outData)
{
  std::string line = original;
  trim(line);

  if (line.size() < 5 || line[0] != '(' || line[1] != ':' ||
    line[line.size() - 2] != ':' || line[line.size() - 1] != ')') {
    return false;
  }

  std::string inner = ":" + line.substr(2, line.size() - 4);
  trim(inner);
  if (inner.empty() || inner.back() != ':') {
    inner.push_back(':');
  }

  bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
  size_t pos = 0;

  while (true) {
    const size_t startColon = inner.find(':', pos);
    if (startColon == std::string::npos) {
      break;
    }
    const size_t spacePos = inner.find(' ', startColon + 1);
    if (spacePos == std::string::npos) {
      break;
    }
    const size_t endColon = findNextColonOutsideQuotes(inner, spacePos + 1);
    if (endColon == std::string::npos) {
      break;
    }

    std::string fieldName = inner.substr(startColon + 1, spacePos - (startColon + 1));
    trim(fieldName);

    std::string fieldValue = inner.substr(spacePos + 1, endColon - (spacePos + 1));
    trim(fieldValue);

    pos = endColon;

    if (fieldName == "key1") {
      double val = 0.0;
      if (!parseDoubleLit(fieldValue, val)) {
        return false;
      }
      outData.key1_ = val;
      hasKey1 = true;
    }
    else if (fieldName == "key2") {
      unsigned long long val = 0ULL;
      if (!parseUllLit(fieldValue, val)) {
        return false;
      }
      outData.key2_ = val;
      hasKey2 = true;
    }
    else if (fieldName == "key3") {
      if (fieldValue.size() < 2 ||
        fieldValue.front() != '"' ||
        fieldValue.back() != '"') {
        return false;
      }
      outData.key3_ = fieldValue.substr(1, fieldValue.size() - 2);
      hasKey3 = true;
    }
  }
  return hasKey1 && hasKey2 && hasKey3;
}

std::istream& operator>>(std::istream& in, DataStruct_t& data)
{
  std::string line;
  if (!std::getline(in, line)) {
    in.setstate(std::ios::failbit);
    return in;
  }

  DataStruct_t temp;
  if (!parseLine(line, temp)) {
    in.setstate(std::ios::failbit);
    return in;
  }

  data = temp;
  return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct_t& data)
{
  out << "(:key1 " << formatDoubleLit(data.key1_)
    << ":key2 " << data.key2_ << "ull"
    << ":key3 \"" << data.key3_ << "\":)";
  return out;
}

bool dataStructComparator(const DataStruct_t& lhs, const DataStruct_t& rhs)
{
  if (lhs.key1_ != rhs.key1_) {
    return lhs.key1_ < rhs.key1_;
  }
  if (lhs.key2_ != rhs.key2_) {
    return lhs.key2_ < rhs.key2_;
  }
  return lhs.key3_.size() < rhs.key3_.size();
}

#include "DataStruct.h"
#include <regex>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <iomanip>

namespace mydatastruct {

  bool isValidDouble(const std::string& str)
  {
    std::regex dbl_pattern(R"([1-9](\.\d+)?[eE][+-]\d+)");
    std::smatch match;
    if (std::regex_search(str, match, dbl_pattern) && match[0] == str) {
      return true;
    }
    return false;
  }

  bool isValidChar(const std::string& str)
  {
    return str.size() == 3 && str.front() == '\'' && str.back() == '\'';
  }

  bool isValidStr(const std::string& str)
  {
    return !str.empty() && str.front() == '"' && str.back() == '"';
  }

  std::string SCI_DBL(double num)
  {
    std::ostringstream out;
    out << std::scientific << std::setprecision(6) << num;
    std::string str = out.str();

    size_t e = str.find('e');
    std::string mantissa = str.substr(0, e);
    std::string exponent = str.substr(e + 1);

    while (mantissa.back() == '0') {
      mantissa.pop_back();
    }
    if (mantissa.back() == '.') {
      mantissa += '0';
    }

    char sign = exponent[0];
    size_t start = 1;
    while (start < exponent.size() and exponent[start] == '0') {
      ++start;
    }
    std::string new_e = exponent.substr(start);
    if (new_e.empty()) {
      new_e = "0";
    }

    return mantissa + 'e' + sign + new_e;
  }

  DataStruct::DataStruct() = default;

  DataStruct::DataStruct(const std::string& str)
  {
    const std::regex INSIDE(R"(\(\:(.*?)\:\))");
    std::smatch m, m1, m2, m3;
    if (std::regex_search(str, m, INSIDE)) {
      std::string result = m[1];

      const std::regex KEY_1_PATTERN(R"(key1\s+([^:]+))");
      const std::regex KEY_2_PATTERN(R"(key2\s+([^:]+))");
      const std::regex KEY_3_PATTERN(R"(key3\s*\"([^\"]+)\")");

      if (std::regex_search(result, m1, KEY_1_PATTERN) and isValidDouble(m1[1].str())) {
         key1 = std::stof(m1[1].str());
      } else {
        valid = false;
      }

      if (std::regex_search(result, m2, KEY_2_PATTERN) and isValidChar(m2[1].str())) {
        key2 = m2[1].str()[1];
      } else {
        valid = false;
      }

      if (std::regex_search(result, m3, KEY_3_PATTERN)) {
        key3 = m3[1];
      } else {
        valid = false;
      }
    } else {
      throw std::invalid_argument("Could not find pattern (:...:)");
    }
  }

  std::istream& operator>>(std::istream& in, DataStruct& ds)
  {
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) {
        in.setstate(std::ios::failbit);
        return in;
      }

      DataStruct temp(line);
      if (temp.valid) {
        ds = temp;
        return in;
      }
    }

    in.setstate(std::ios::failbit);
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& ds)
  {
    out << "(:key1 " << SCI_DBL(ds.key1) << ":key2 '" << ds.key2 << "':key3 \"" << ds.key3 << "\":)";
    return out;
  }

  void fillvector(std::vector<DataStruct>& vec)
  {
    std::copy(std::istream_iterator<DataStruct>(std::cin),
              std::istream_iterator<DataStruct>(),
              std::back_inserter(vec));
  }

  bool datastructSort(const DataStruct &el1, const DataStruct &el2)
  {
    if (el1.key1 != el2.key1) {
      return el1.key1 < el2.key1;
    }
    if (el1.key2 != el2.key2) {
      return el1.key2 < el2.key2;
    }
    return el1.key3 < el2.key3;
  }

}

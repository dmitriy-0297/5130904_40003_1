#include "../include/Command.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <regex>

void begin()
{
  std::cout << "Enter the command or type \"HELP\" to see available commands\n";
  std::vector<Command> commands;
  std::copy_if(std::istream_iterator<Command>(std::cin),
               std::istream_iterator<Command>(),
               std::back_inserter(commands),
               [](const Command &command)
               {
                 return handleCommand(command);
               }
    );
  std::cout << "\n[EOF detected. Program terminated.]\n";
}

bool handleCommand(const Command &command)
{
  StreamGuard streamGuard(std::cout);
  static const auto encodeRegex = std::regex(R"(ENCODE (.+) (.+))");
  static const auto decodeRegex = std::regex(R"(DECODE (.+) (.+))");
  static const auto bothRegex = std::regex(R"(BOTH (.+) (.+) (.+))");
  static const auto helpRegex = std::regex(R"(HELP)");

  std::smatch match;
  if (std::regex_match(command.data, match, encodeRegex))
  {
    const std::string toEncodeFileName = match[1];
    const std::string encodedFileName = match[2];
    Encoder::encode(toEncodeFileName, encodedFileName);
  }
  else if (std::regex_match(command.data, match, decodeRegex))
  {
    const std::string encodedFileName = match[1];
    const std::string decodedFileName = match[2];
    Decoder::decode(encodedFileName, decodedFileName);
  }
  else if (std::regex_match(command.data, match, bothRegex))
  {
    const std::string toEncodeFileName = match[1];
    const std::string encodedFileName = match[2];
    const std::string decodedFileName = match[3];
    Encoder::encode(toEncodeFileName, encodedFileName);
    Decoder::decode(encodedFileName, decodedFileName);
  }
  else if (std::regex_match(command.data, helpRegex))
  {
    std::cout << "Available commands:\n" <<
      "ENCODE <file to encode (.txt)> <encoded file (.bin)>\n" <<
      "DECODE <encoded file (.bin)> <decoded file (.txt)>\n" <<
      "BOTH <file to encode (.txt)> <encoded file (.bin)> <decoded file (.txt)>\n";
  }
  else
  {
    std::cout << INVALID_COMMAND;
    std::cout <<
      "Enter the command or type \"HELP\" to see available commands\n";
    return false;
  }
  std::cout << "Enter the command or type \"HELP\" to see available commands\n";
  return true;
}

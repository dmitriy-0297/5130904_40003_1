#ifndef COMMAND_H
#define COMMAND_H
#include <istream>
#include <string>
#include "StreamGuard.h"
#include "Encoder.h"
#include "Decoder.h"


const std::string INVALID_COMMAND = "<INVALID COMMAND>\n";

struct Command
{
    std::string data;

    friend std::istream& operator>>(std::istream& in, Command& command)
    {
        const std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        std::getline(in, command.data);

        return in;
    }
};

void begin();
bool handleCommand(const Command& command);

#endif //COMMAND_H

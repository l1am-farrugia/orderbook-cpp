#pragma once

#include "command.h"

#include <optional>
#include <string>
#include <vector>

namespace ob
{
    // parses a script file into commands
    // format:
    //   add <id> <buy|sell> <price_ticks> <qty>
    //   cancel <id>
    std::optional<std::vector<Command>> load_script(const std::string& path);

    // parses a single script line
    std::optional<Command> parse_script_line(const std::string& line);
}

#include "script.h"

#include <fstream>
#include <sstream>

namespace ob
{
    static bool is_blank_or_comment(const std::string& line)
    {
        for (char c : line)
        {
            if (c == '#')
            {
                return true;
            }
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
            {
                return false;
            }
        }
        return true;
    }

    std::optional<Command> parse_script_line(const std::string& line)
    {
        // simple tokenizer parsing
        std::istringstream iss(line);

        std::string kind;
        if (!(iss >> kind))
        {
            return std::nullopt;
        }

        if (kind == "add")
        {
            std::uint64_t id_u {};
            std::string side_s;
            std::int64_t px {};
            std::int64_t qty {};

            if (!(iss >> id_u >> side_s >> px >> qty))
            {
                return std::nullopt;
            }

            Side side {};
            if (side_s == "buy" || side_s == "b")
            {
                side = Side::Buy;
            }
            else if (side_s == "sell" || side_s == "s")
            {
                side = Side::Sell;
            }
            else
            {
                return std::nullopt;
            }

            return Command::add_limit(static_cast<OrderId>(id_u), side, px, qty);
        }

        if (kind == "cancel")
        {
            std::uint64_t id_u {};
            if (!(iss >> id_u))
            {
                return std::nullopt;
            }
            return Command::cancel(static_cast<OrderId>(id_u));
        }

        return std::nullopt;
    }

    std::optional<std::vector<Command>> load_script(const std::string& path)
    {
        std::ifstream in(path);
        if (!in)
        {
            return std::nullopt;
        }

        std::vector<Command> out;

        std::string line;
        while (std::getline(in, line))
        {
            if (is_blank_or_comment(line))
            {
                continue;
            }

            const auto cmd = parse_script_line(line);
            if (!cmd.has_value())
            {
                return std::nullopt;
            }

            out.push_back(*cmd);
        }

        return out;
    }
}

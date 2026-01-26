#include "script.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

namespace ob
{
    static std::string chomp_cr(std::string s)
    {
        // strips windows carriage return if present
        if (!s.empty() && s.back() == '\r')
        {
            s.pop_back();
        }
        return s;
    }

    static std::string to_lower_ascii(std::string s)
    {
        // lowercases keywords for forgiving scripts
        for (char& c : s)
        {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        return s;
    }

    static std::string strip_comment(std::string line)
    {
        // removes trailing comment part starting at '#'
        const auto pos = line.find('#');
        if (pos != std::string::npos)
        {
            line = line.substr(0, pos);
        }
        return line;
    }

    static bool is_blank(const std::string& line)
    {
        for (char c : line)
        {
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
            {
                return false;
            }
        }
        return true;
    }

    std::optional<Command> parse_script_line(const std::string& raw_line)
    {
        // parse one logical line into a command
        const std::string line = strip_comment(chomp_cr(raw_line));

        if (is_blank(line))
        {
            return std::nullopt;
        }

        std::istringstream iss(line);

        std::string kind;
        if (!(iss >> kind))
        {
            return std::nullopt;
        }
        kind = to_lower_ascii(kind);

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

            // reject extra tokens to keep scripts strict
            std::string extra;
            if (iss >> extra)
            {
                return std::nullopt;
            }

            side_s = to_lower_ascii(side_s);

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

            std::string extra;
            if (iss >> extra)
            {
                return std::nullopt;
            }

            return Command::cancel(static_cast<OrderId>(id_u));
        }

        return std::nullopt;
    }

    std::optional<std::vector<Command>> load_script(const std::string& path)
    {
        // loads a script file into commands
        std::ifstream in(path, std::ios::binary);
        if (!in)
        {
            std::cerr << "script open failed path=" << path << "\n";
            return std::nullopt;
        }

        std::vector<Command> out;

        std::string line;
        std::size_t line_no { 0 };

        while (std::getline(in, line))
        {
            ++line_no;

            // keep original line for diagnostics
            const std::string original = chomp_cr(line);

            const auto cmd = parse_script_line(original);
            if (!cmd.has_value())
            {
                // ignore blanks and pure comments
                const std::string stripped = strip_comment(original);
                if (is_blank(stripped))
                {
                    continue;
                }

                std::cerr << "script parse failed line=" << line_no << "\n";
                std::cerr << "line: " << original << "\n";
                return std::nullopt;
            }

            out.push_back(*cmd);
        }

        return out;
    }
}

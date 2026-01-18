#include "engine.h"

#include "event_io.h"
#include "script.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static void print_usage()
{
    std::cout << "usage:\n";
    std::cout << "  ob_sim --script <path>\n";
    std::cout << "  ob_sim --script <path> --record <event_log>\n";
    std::cout << "  ob_sim --replay <path> --events <event_log>\n";
}

static bool read_all_lines(const std::string& path, std::vector<std::string>& out_lines)
{
    std::ifstream in(path);
    if (!in)
    {
        return false;
    }

    std::string line;
    while (std::getline(in, line))
    {
        if (!line.empty())
        {
            out_lines.push_back(line);
        }
    }
    return true;
}

static int run_script(const std::string& script_path, const std::string& record_path)
{
    const auto cmds_opt = ob::load_script(script_path);
    if (!cmds_opt.has_value())
    {
        std::cerr << "failed to load script\n";
        return 1;
    }

    ob::Engine eng;

    if (!record_path.empty())
    {
        if (!eng.start_event_log(record_path))
        {
            std::cerr << "failed to open event log\n";
            return 1;
        }
    }

    for (const auto& cmd : *cmds_opt)
    {
        const auto events = eng.apply(cmd);
        for (const auto& e : events)
        {
            std::cout << ob::event_to_line(e) << "\n";
        }
    }

    eng.stop_event_log();
    return 0;
}

static int replay_script(const std::string& script_path, const std::string& events_path)
{
    const auto cmds_opt = ob::load_script(script_path);
    if (!cmds_opt.has_value())
    {
        std::cerr << "failed to load script\n";
        return 1;
    }

    std::vector<std::string> expected_lines;
    if (!read_all_lines(events_path, expected_lines))
    {
        std::cerr << "failed to read events file\n";
        return 1;
    }

    ob::Engine eng;

    std::vector<std::string> actual_lines;
    {
        const auto events = eng.apply_all(*cmds_opt);
        actual_lines.reserve(events.size());

        for (const auto& e : events)
        {
            actual_lines.push_back(ob::event_to_line(e));
        }
    }

    if (actual_lines.size() != expected_lines.size())
    {
        std::cerr << "mismatch lines count expected=" << expected_lines.size()
                  << " actual=" << actual_lines.size() << "\n";
        return 2;
    }

    for (std::size_t i = 0; i < actual_lines.size(); ++i)
    {
        if (actual_lines[i] != expected_lines[i])
        {
            std::cerr << "mismatch at line " << i << "\n";
            std::cerr << "expected: " << expected_lines[i] << "\n";
            std::cerr << "actual:   " << actual_lines[i] << "\n";
            return 3;
        }
    }

    std::cout << "replay ok\n";
    return 0;
}

int main(int argc, char** argv)
{
    std::string script_path;
    std::string record_path;

    bool replay = false;
    std::string replay_script_path;
    std::string replay_events_path;

    for (int i = 1; i < argc; ++i)
    {
        const std::string a = argv[i];

        if (a == "--script" && i + 1 < argc)
        {
            script_path = argv[++i];
        }
        else if (a == "--record" && i + 1 < argc)
        {
            record_path = argv[++i];
        }
        else if (a == "--replay" && i + 1 < argc)
        {
            replay = true;
            replay_script_path = argv[++i];
        }
        else if (a == "--events" && i + 1 < argc)
        {
            replay_events_path = argv[++i];
        }
        else
        {
            print_usage();
            return 1;
        }
    }

    if (replay)
    {
        if (replay_script_path.empty() || replay_events_path.empty())
        {
            print_usage();
            return 1;
        }
        return replay_script(replay_script_path, replay_events_path);
    }

    if (script_path.empty())
    {
        print_usage();
        return 1;
    }

    return run_script(script_path, record_path);
}

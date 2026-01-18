#pragma once

#include "event.h"

#include <optional>
#include <string>

namespace ob
{
    // converts an event to a stable single line format
    std::string event_to_line(const Event& e);

    // parses one line into an event, returns nullopt on failure
    std::optional<Event> line_to_event(const std::string& line);

    // helpers used by both logging and replay
    const char* event_type_to_string(EventType t);
    std::optional<EventType> string_to_event_type(const std::string& s);

    const char* side_to_string(Side s);
    std::optional<Side> string_to_side(const std::string& s);
}

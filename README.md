# Exchange Order Book Simulator (C++)

This is a deterministic limit order book simulator with price time priority matching, event logging, and deterministic replay. 
Built with Cmake and tested with GoogleTest.

---

## What this project is

It implements a single instrument exchange style **limit order book**:

- It accepts limit orders (add).
- Supports cancel by id.
- Matches orders using **price priority then fifo time priority** within each price level
- Emits a deterministic stream of events.
- Can record events to a file and later replay a scrpt and verify the event stream matches exactly.
- Includes a small benchmark mode to measure basic throughput .

---

## Folder layout

From the root:

- `src/` library and app source
- `tests/` googletest suite
- `examples/` sample scripts and recorded event logs
- `build/` build output (not committed)

---

## Prerequisites

### Windows 11 tools

you need these installed and working:

- **cmake** (3 20+)
- **g++** (mingw w64)
- **git**

---

## Using as a library

The core engine can be driven directly from c++ by sending commands and consuming events:

```cpp
#include "engine.h"
#include "event_io.h"
#include <iostream>

int main()
{
    ob::Engine eng;

    auto e1 = eng.apply(ob::Command::add_limit(1, ob::Side::Sell, 100, 10));
    auto e2 = eng.apply(ob::Command::add_limit(2, ob::Side::Buy, 150, 4));

    for (const auto& e : e1) { std::cout << ob::event_to_line(e) << "\n"; }
    for (const auto& e : e2) { std::cout << ob::event_to_line(e) << "\n"; }

    return 0;
}

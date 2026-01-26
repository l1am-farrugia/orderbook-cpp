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

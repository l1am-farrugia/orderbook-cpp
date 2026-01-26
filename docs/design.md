# Design Notes

## Matching Rules
- Limit orders are matched using price priority then fifo time priority.
- Buy orders match the lowest ask prices first while price <= buy limit.
- Sell orders match the highest bid prices first while price >= sell limit.
- Trades execute at the maker price.
- Partial fils are supported and remaining qty stays resting or becomes resting.

## Data Structures
- Bids and asks use std::map for determinitic best price selection.
  - Bids are sorted highest to lowest.
  - Asks are sorted lowest to highest.
- Each price level stores orders in std::list to keep fifo and stable iterators.
- An id index maps order id to a locator (side price and list iterator) for fast cancel.

## Determinism Strategy
- Script commands are applied in order.
- Events are emitted in a deterministic order from the matching loop.
- Event logs use a stable single line key value format.
- Replay will rerun the script and compare event lines.

## Invariants
- Index size matches total number of resting orders across all levels.
- No empty price levels remain.
- All resting orders have qty > 0 and seq != 0.
- Each order id in levels exists in index and the locator points to the same order.

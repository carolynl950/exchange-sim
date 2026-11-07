# Exchange Simulator

A C++20 exchange simulator that replays one day of historical market data and trades, lets a trading strategy submit and cancel orders, decides when those orders fill, and dispatches order events back to the strategy.

> **Status:** Work in progress. Sections marked **TODO** are filled in as the design decisions are made and the code is written.

## What it does

The simulator takes three inputs:

1. Incremental market data updates (order book changes)
2. Market trades
3. Orders submitted by a strategy

It maintains the state of the market, simulates how strategy orders interact with that market, and sends events back to the strategy:

- Order accepted
- Order canceled
- Partial fill
- Full fill

## Architecture

```
CSV files ──► Parser ──► Event stream (sorted by time)
                              │
                              ▼
                     ┌─────────────────┐
  Strategy ──orders─►│  Simulation     │──events──► Strategy
  (your code)        │  engine         │  (accept, cancel,
                     │                 │   partial fill, fill)
                     │  Market book    │
                     │  Order manager  │
                     │  Matching logic │
                     └─────────────────┘
                              │
                              ▼
                  Logs, summary, validation checks
```

### Components

| Component | Responsibility |
|---|---|
| Core types | `Price` and `Qty` as integers (no floating point), `Side`, `OrderId`, `Timestamp`, event structs |
| Parser | Converts CSV rows into internal events. The only part that depends on the file format |
| Replay loop | Merges book updates and trades into one time-ordered stream and feeds the engine |
| Market book | Aggregate size per price level on each side, built from incremental updates. Represents the real market, not strategy orders |
| Order manager | Tracks strategy orders and their states (new, accepted, partially filled, filled, canceled) |
| Matching and fill logic | Aggressive orders walk price levels and fill at each level's price. Passive orders fill based on queue position |
| Latency model | Delay between the strategy sending an order and the engine seeing it, and between the engine and the strategy receiving the result |
| Strategy interface | Callbacks the engine invokes on market updates and order events, plus an example strategy |
| Validation | Invariant checks run during replay (see below) |

## Design decisions and assumptions

These are the assumptions the simulator makes. Each one affects results, so they are listed explicitly.

| Topic | Decision |
|---|---|
| Language | C++20 |
| Price and quantity representation | Integers (ticks or scaled). Floating point is avoided because equal prices can compare unequal. Scale: **TODO** (depends on the data) |
| Book data structure | **TODO** (`std::map` vs. flat array indexed by tick) |
| Book update semantics | **TODO** (absolute level sizes vs. deltas, snapshot vs. empty start; depends on the data) |
| Event ordering on equal timestamps | **TODO** |
| Latency model | **TODO** (fixed delay each way vs. separate delays) |
| Queue position for passive orders | **TODO** (assumed to join the back of the queue at its price level; document how trades and cancels ahead of the order are handled) |
| Cancels ahead of the order | **TODO** (worst case, proportional, or other) |
| Passive fill rule | **TODO** |
| Self-impact on the market | **TODO** (strategy orders do not move the replayed market data, so the simulator is not a full market-impact model) |

### Known limitations

- Historical data is replayed as-is. Strategy orders do not change what other participants did, so fills are an estimate.
- With L2 data, individual orders are not visible, so queue position is modeled rather than observed.
- **TODO:** add limitations as they are discovered.

## Validation

The simulator checks the following during replay:

- The book is never crossed (best bid is always below best ask). A crossed book indicates a bad update, a missed delete, or an assumption error in how updates are applied
- Quantity conservation: filled quantity plus remaining quantity equals original quantity
- No fill occurs at a price worse than the order's limit
- Timestamps are non-decreasing
- Position and PnL reconcile with the fill log
- The worked examples from the assignment (resting a bid, walking multiple price levels, resting the leftover quantity) reproduce the expected book and fills exactly

**TODO:** list the checks actually implemented and any violations found on the real data.

## Build

Requires a C++20 compiler (g++ 11+ or clang++ 14+) and CMake.

```bash
# Release build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Debug build with sanitizers
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined"
cmake --build build-debug
```

## Run

```bash
./build/exchange_sim --market-data data/incremental_book_L2_BTC-USD.csv.gz --trades data/trades_BTC-USD.csv.gz
```

**TODO:** confirm executable name, flags, and the real file names.

## Output

**TODO:** finalize. Planned outputs:

- Order and fill log (accepts, cancels, partial and full fills)
- Summary statistics (orders sent, fill rate, final position, PnL)
- Validation report
- Replay throughput (events per second)

## Project layout

```
exchange-sim/
  CMakeLists.txt
  README.md
  src/        source code
  data/       input CSVs (not distributed)
  out/        logs and summaries
```

## Use of AI

AI tools were used during development for design discussion, explanation of exchange concepts, and code review. **TODO:** describe specifically what was AI-assisted, what was written independently, and how AI-generated code was verified.


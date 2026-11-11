# tpch_join

This is the starter project for the exercises on joining the TPC-H tables `lineitem` and `orders`.
The parts you implement are marked with `TODO` in `main.cpp` and `mapped_file.hpp`.

## Requirements

- A C++20 compiler (GCC 11 or newer, or Clang 14 or newer)
- CMake 3.16 or newer
- For profiling: Linux with `perf` (see below)

## Build

From the project folder:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
```

After changing the code, only the second command is needed.

**In CLion:** open the folder as a project. Under *Settings → Build, Execution, Deployment → CMake*,
add a profile with build type `RelWithDebInfo`, set its *Build directory* to `build`, and select the
profile in the toolbar.

Always measure with `RelWithDebInfo`: it is optimized like a release build, but keeps the debug
information that `perf` needs to show function names and source lines. A `Debug` build is much
slower and gives misleading timings.

## Run

The program takes the folder containing `lineitem.tbl` and `orders.tbl`, and the variant to run
(`nested`, `hash` or `mmap`, see the exercise sheet):

```bash
build/tpch_join tpch/sf0_01 nested
```

The timings are printed to stderr, the result to stdout. Use these results to check your
implementation:

| Scale factor | Folder         | Result     |
|--------------|----------------|------------|
| 0.01         | `tpch/sf0_01`  | 307608     |
| 0.02         | `tpch/sf0_02`  | 609877     |


## perf (Linux only)

Install `perf` and allow it to measure your programs (the two settings reset on reboot):

```bash
sudo apt install linux-tools-$(uname -r) linux-tools-generic
sudo sysctl -w kernel.perf_event_paranoid=1   # allow measuring your own programs
sudo sysctl -w kernel.kptr_restrict=0         # show kernel function names
```

Count events with `perf stat`, or record where the time goes with `perf record`:

```bash
perf stat build/tpch_join tpch/sf0_01 nested
perf record -g build/tpch_join tpch/sf0_01 nested
perf report --no-children
```

In `perf report`, select a function with the arrow keys and press `a` to see its hottest
instructions; `q` goes back.

# tpch_join

This is the starter project for the exercises on joining the TPC-H tables `lineitem` and `orders`.
It continues from the solution of the previous sheet. The parts you implement are marked with
`TODO` in `main.cpp`.

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
information that `perf` needs to show function names and source lines.

## Data

The folders `tpch/sf0_01` and `tpch/sf0_02` (scale factors 0.01 and 0.02) are included.
Use them to check that your implementation is correct.

For scale factor 1, download `lineitem.tbl` and `orders.tbl` of the TPC-H data set from Moodle and
put them into a new folder `tpch/sf1`:

```
tpch/sf1/lineitem.tbl
tpch/sf1/orders.tbl
```

**Use scale factor 1 for all profiling and timing, in particular for the speedup of `parallel`.**
With the small scale factors, the program finishes within a few milliseconds. That is too short for
`perf` to collect enough samples, and too short to measure timings and speedups reliably.

## Run

The program takes the folder containing `lineitem.tbl` and `orders.tbl`, the variant to run, and
for `parallel` optionally the number of threads (default: all cores):

```bash
build/tpch_join tpch/sf1 swar
build/tpch_join tpch/sf1 parallel 4
```

| Variant    | Sheet                          |
|------------|--------------------------------|
| `nested`   | Sheet 3                        |
| `hash`     | Sheet 3                        |
| `mmap`     | Sheet 3                        |
| `swar`     | Sheet 4, Exercise 1            |
| `parallel` | Sheet 4, Exercise 2            |

The timings are printed to stderr, the result to stdout. Use these results to check your
implementation:

| Scale factor | Folder         | Result     |
|--------------|----------------|------------|
| 0.01         | `tpch/sf0_01`  | 307608     |
| 0.02         | `tpch/sf0_02`  | 609877     |
| 1            | `tpch/sf1`     | 30656613   |

Do not run `nested` on scale factor 1: it would take hours.

## perf (Linux only)

Install `perf` and allow it to measure your programs (the two settings reset on reboot):

```bash
sudo apt install linux-tools-$(uname -r) linux-tools-generic
sudo sysctl -w kernel.perf_event_paranoid=1   # allow measuring your own programs
sudo sysctl -w kernel.kptr_restrict=0         # show kernel function names
```

Count events with `perf stat`, or record where the time goes with `perf record`:

```bash
perf stat build/tpch_join tpch/sf1 mmap
perf record build/tpch_join tpch/sf1 mmap
perf report --no-children
```

In `perf report`, select a function with the arrow keys and press `a` to see its hottest
instructions; `q` goes back. If that view fails, use the text version instead:

```bash
perf annotate --stdio | less
```

## Troubleshooting

- **`Could not open file`**: the data folder path is wrong. It is relative to the folder you run
  the command from.
- **`perf` reports missing permissions**: run the two `sysctl` commands above again; they reset on reboot.
- **`Cannot load tips.txt file`** or **`WARNING: event 'N/A' not valid`**: harmless, ignore them.
- **macOS**: `perf` is not available. The *Time Profiler* in Instruments (part of Xcode) gives a
  similar view, or use a Linux machine for the profiling parts.

# tinydb

A tiny database engine for the first exercise sheet.
It loads the TPC-H `lineitem` table (scale factor 0.1) from `data/lineitem.tbl`,
so always start it from this folder (CLion does this by default).

## Build and run

**CLion:** open this folder and run `tinydb`.
For time measurements, add a Release profile under
*Settings → Build, Execution, Deployment → CMake* and select it.

**Command line:**

```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/tinydb
```

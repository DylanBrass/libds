# DataStructures

A from-scratch collection of data structures in C, written to understand how they actually work in memory.
Built as a static library so it can be reused by other projects (for example, my [http_server](https://github.com/DylanBrass/http_server)).

## Roadmap

Checked boxes are implemented and tested.

### Linear structures
- [x] Dynamic array (resizable vector)
- [ ] Ring buffer / bounded queue
- [ ] Thread-safe queue (mutex + condition variables)
- [ ] Singly linked list
- [ ] Doubly linked list
- [ ] Stack (array-backed)
- [ ] Stack (list-backed)
- [ ] Queue (list-backed)
- [ ] Deque

### Hash-based
- [ ] Hash table (separate chaining)
- [ ] Hash table (open addressing)
- [ ] Hash set

### Trees
- [ ] Binary search tree
- [ ] AVL tree
- [ ] Red-black tree
- [ ] Binary heap / priority queue
- [ ] Trie
- [ ] B-tree

### Graphs
- [ ] Graph (adjacency list)
- [ ] Graph (adjacency matrix)
- [ ] BFS and DFS
- [ ] Dijkstra's shortest path (uses the priority queue)

### Stretch goals
- [ ] Skip list
- [ ] Bloom filter
- [ ] LRU cache (hash table + doubly linked list)
- [ ] Union-find (disjoint set)

## Project layout

```
DataStructures/
├── CMakeLists.txt
├── include/ds/     public headers (what other projects include)
├── src/            implementations
└── tests/          one test executable per structure
```

## Conventions

- Every public function is prefixed with `ds_` (e.g. `ds_vec_push`) since C has no namespaces.
- Structures are generic: they store an element size and use `memcpy`, or hold `void *` where that makes more sense.
- Every function that can fail (allocation, invalid argument) returns an error code instead of crashing.
- Ownership is explicit: whoever calls `*_init` / `*_create` is responsible for calling the matching `*_destroy`.

## Building and testing

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Tests are only built when this is the top-level project, so they won't be pulled into projects that consume the library.

## Using the library in another project

### Option A: `add_subdirectory` (or a git submodule)

```cmake
add_subdirectory(path/to/DataStructures ${CMAKE_BINARY_DIR}/ds)
target_link_libraries(my_target PUBLIC DataStructures::ds)
```

### Option B: `FetchContent`

```cmake
include(FetchContent)
FetchContent_Declare(DataStructures
        GIT_REPOSITORY https://github.com/<user>/DataStructures.git
        GIT_TAG main)
FetchContent_MakeAvailable(DataStructures)
target_link_libraries(my_target PUBLIC DataStructures::ds)
```

Then in your C code:

```c
#include <ds/vector.h>
```

## Requirements

- CMake 4.1 or newer
- A C23-capable compiler (GCC or Clang)

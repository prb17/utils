# CLAUDE.md

Guidance for working in this repository.

## What this is

`prb17/utils` is a from-scratch C++17 utility library: containers (array, stack,
queue, set, map, graph), algorithms, a JSON parser, a logger, timing helpers,
GoF patterns, and a small JSON-driven test validator. Everything lives under the
`prb17::utils` namespace and is header-only (templates defined in `.hh` headers).

## Cloning

`find-cmakes/` is a git submodule (it provides the custom `find_package`
modules). Clone with submodules:

```
git clone --recurse-submodules <url>
```

If you already cloned without it:

```
git submodule update --init --recursive
```

Do NOT modify anything inside `find-cmakes/` — it is a submodule.

## Build dependencies

- `cmake` (>= 3.16)
- `libjsoncpp-dev` (the JSON parser wraps JsonCpp)

On Debian/Ubuntu: `apt-get install -y cmake libjsoncpp-dev`.

## Build

```
mkdir -p build && cd build && cmake -DBUILD_UTILS_TESTS=ON .. && make
```

`-DBUILD_UTILS_TESTS=ON` builds the test executables.

## Running tests

Each test executable takes one or more JSON config files describing the test
cases. For the graph tests:

```
./graphs_test ../structures/graphs/tests/config/graph_test_functions.json
```

The validator prints a per-test passed/failed tally and a final total.

## Architecture: structures vs. algorithms

Keep this layering strict:

- **Data structures** (`structures/`) own **storage and access only** —
  constructing, adding, removing, and getting elements.
- Anything that **operates on** a structure — searching, traversing, rendering
  — is an **algorithm** living in its own namespace under `algorithms/`, the way
  the STL `<algorithm>` header is separate from its containers.
- The dependency only goes **algorithms -> structures**. Structures must NOT
  depend on `algorithms/`.

For example, graph traversal (`dfs`, `bfs`), `to_string`, and
`to_adjacency_list` live in `algorithms/graphs/traverse.hh` under
`prb17::utils::algorithms::graphs`, not on the `graph` class. Visited-membership
checks there go through `prb17::utils::algorithms::search::find` so the search
algorithm stays swappable rather than being baked into a container.

## Style

- 4-space indentation.
- Nested `prb17::utils::structures` (or `::algorithms::...`) namespaces.
- Templates are declared and defined in headers.

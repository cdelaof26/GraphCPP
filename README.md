# GraphCPP

GraphCPP is a C++ project that makes use of SDL2 
to animate algorithms for graphs (mathematical structure) such 
as Dijkstra (find the shortest path between nodes), 
Prim and Kruskal (find minimum spanning tree). 

### Dependencies
- C++ 11
- SDL2

### Compiling
```bash
g++ --std=c++11 *.cpp fonts/*.cpp algorithms/*.cpp -o graph -lSDL2
```

### Usage
```bash
# Kruskal algorithm sample 
./graph sample.txt

# Dijkstra algorithm sample 
./graph dsample.txt

# Any file
./graph path/to/file
```

### Defining matrix
To define a matrix, it is necessary to create
a plain text file with the following format:
<pre>
[Algorithm name]
[Adjacency matrix]
</pre>

> [!NOTE]  
> Check `sample.txt` and `dsample.txt` for reference

> [!WARNING]  
> The values in the adjacency matrix must be integers

`[Algorithm name]` can be one of the three values
- Kruskal or just `k`, 
- Prim or just `p`,
- Dijkstra or just `d`
  - **Note**: Use -1 in the adjacency matrix to 
    represent `INF`.

### Licensing
Licensed under the [MIT License](LICENSE). Copyright 2025 MiguelPereZero and @cdelaof26.

### Versioning

#### v0.0.5 Partial Dijkstra implementation
- Added CLI option to read from files

#### v0.0.4 More animations
- Also added support for Prim algorithm

#### v0.0.3 Kruskal implementation and improvements
- `UIButton`
  - Fixed ghost clicks when dragging
  - Improved hover detection
- UI elements render logic translated to `Window.h`
- `UIText` is now a `Shape`
- Added states to Kruskal
- Matrix data is now loaded from a file

#### v0.0.2 Graph representation
- Added button (buggy)
- Added Algoritmos from MiguelPereZero
- Added [LICENSE](LICENSE)

#### v0.0.1 Initial project

# GraphCPP

GraphCPP is a C++ project that makes use of SDL2 
to animate algorithms for graphs (mathematical structure) such 
as Dijkstra (find the shortest path between nodes), 
Prim and Kruskal (find minimum spanning tree). 

### Dependencies
- C++ 11
- SDL2

### Running
```bash
g++ --std=c++11 *.cpp fonts/*.cpp algorithms/*.cpp -o graph -lSDL2 && ./graph
```

### Defining matrix
To define a matrix, it is necessary making edits
to [`sample.txt`](sample.txt), at top must be the 
name of the algorithm to use (Kruskal `k`, 
Prim `p` or Dijkstra `d`) and, starting the next 
line, all the square matrix data.

### Licensing
Licensed under the [MIT License](LICENSE). Copyright 2025 MiguelPereZero and @cdelaof26.

### Versioning

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

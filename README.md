## ir [WIP]

### Planned features:
- TF-IDF based search
- Numerical range filters
- k-nearest neighbour vector search

### Setup:
- Get bazel 0.20
- run `bazel query //...` to see build targets
- run `bazel run //cpp/server:server` to start the server

### Components:
#### Persistent Matrices:
- An mmap implementation of dense eigen matrices is available in cpp/ds/persistent_matrix.h


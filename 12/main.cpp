#include "input.h"
#include "matrix.h"
#include <iostream>
#include <set>


// recursively traverse all paths in this graph, visiting small caves at most once
// returns the number of unique paths found
// the "visited" set is copy passed each time to create a unique context per recursive search
void traverse_paths(AdjMatrix& matrix, size_t start, const size_t end, std::set<size_t> visited, std::size_t& n_paths, bool double_visited) {

  // we've hit the end! we're done!
  if (start == end) {
    n_paths++;
    return;
  }

  // ok... we're not at the end.
  // did we just enter a small cave?
  if (matrix.is_small(start)) {
    visited.insert(start);
  }

  // check all our neighbors, if we haven't visitd them yet
  const auto& potential_neighbors = matrix[start];
  // boost could help us here with its indexed adapter...

  for (size_t i = 0; i < potential_neighbors.size(); i++) {
    if (!potential_neighbors[i] or i == matrix.get_start_idx()) {
      continue;
    }

    // ok there's a path! but have we visited this one yet?
    // we're allowed 1 double visit
    if (visited.find(i) != visited.end()) {
      if (double_visited) {
        continue;
      } else {
        // continue, but note that we can't do this again in this context
        traverse_paths(matrix, i, end, visited, n_paths, true);
      }
    } else {
      // well there's a path and it's either a big cave, or a (less than twice visited) small cave; check it out!
      // continue, passing on whatever state we're currently in
      traverse_paths(matrix, i, end, visited, n_paths, double_visited);
    }
  }
}

void p1(AdjMatrix& adjm) {
  // where we are
  size_t start_idx = adjm.get_start_idx();
  // where we're going
  size_t end_idx = adjm.get_end_idx();
  // if we've visited this node (assume nodes can be visited infinite times unless marked here)
  // consider our start node "visited" by default
  std::set<size_t> visited{adjm.get_start_idx()};

  std::size_t n_paths = 0;
  std::list<std::string> path;
  traverse_paths(adjm, start_idx, end_idx, visited, n_paths, false);

  // see previous git commit for P1
  std::cout << "P2: There are " <<  n_paths << " unique paths." << std::endl;
}

int main() {
  // you know... I've been writing C++ professionally for over 3 years and I 
  // don't think I've ever actually had to do bonafide graph traversal
  // let's try an adjacency matrix since we don't need to add/remove nodes...
  // at least for p1....

  AdjMatrix adjm;
  size_t n_paths = 0;

  for (const auto& edge : g_input) {
    adjm.add_edge(std::get<0>(edge), std::get<1>(edge));
  }

  std::cout << "Created matrix!" << std::endl << adjm << std::endl;

  p1(adjm);

  return 0;
}
#include "input.h"
#include <iostream>
#include <list>
#include <tuple>
#include <unordered_map>
#include "boost/functional/hash.hpp"

struct KeyHash {
    std::size_t operator()(const std::tuple<size_t, size_t>& key) const
    {
        return boost::hash_value(key);
    }
};

// ez
void p1(std::list<std::tuple<size_t, size_t>>& low_points) {
  // just check all adjacent points for each point
  for (size_t i = 0; i < N_ROWS; i++) {
    for (size_t j = 0; j < N_COLS; j++) {
      uint8_t cur = g_input[i][j];
      // above
      if (i > 0 && cur >= g_input[i - 1][j]) {
        continue;
      }

      // right
      if (j < 99 && cur >= g_input[i][j + 1]) {
        continue;
      }

      // left
      if (j > 0 && cur >= g_input[i][j - 1]) {
        continue;
      }

      // below
      if (i < 99 && cur >= g_input[i + 1][j]) {
        continue;
      }

      low_points.push_back(std::make_tuple(i, j));
    }
  }

  std::size_t sum = 0;
  for (const auto& p : low_points) {
    sum += g_input[std::get<0>(p)][std::get<1>(p)] + 1;
  }
  std::cout << "p1 " << sum << std::endl;
}


void traverse(const std::tuple<size_t, size_t> p, 
              std::unordered_map<std::tuple<size_t, size_t>, bool, KeyHash>& visited,
              size_t &size) {

  const size_t i = std::get<0>(p);
  const size_t j = std::get<1>(p);

  std::cout << "checking point " << i << " " << j << std::endl;

  if (visited.find(p) != visited.end()) {
    return;
  }

  visited.insert(std::make_pair(p, true));
  if (g_input[i][j] == 9) {
    return;
  }

  size++;

  // above
  if (i > 0) {
    traverse(std::make_tuple(i - 1, j), visited, size);
  }

  // right
  if (j < 99) {
    traverse(std::make_tuple(i, j + 1), visited, size);
  }

  // left
  if (j > 0) {
    traverse(std::make_tuple(i, j - 1), visited, size);
  }

  // below
  if (i < 99) {
    traverse(std::make_tuple(i + 1, j), visited, size);
  }

}

size_t get_basin_size(const std::tuple<size_t, size_t>& p) {
  std::unordered_map<std::tuple<size_t, size_t>, bool, KeyHash> visited;
  size_t size = 0;

  traverse(p, visited, size);

  return size;
}

void p2(const std::list<std::tuple<size_t, size_t>>& low_points) {
  std::list<size_t> basin_sizes;
  for (const auto& p : low_points) {
    basin_sizes.push_back(get_basin_size(p));
  }

  basin_sizes.sort();

  std::list<size_t>::reverse_iterator rit = basin_sizes.rbegin();

  size_t prod = 1;
  for (size_t i = 0; i < 3; i++) {
    std::cout << (*rit) << std::endl;
    prod = prod * (*rit);
    rit++;
  }
  std::cout << "product: " << prod << std::endl;
}

int main() {
  std::list<std::tuple<size_t, size_t>> low_points;
  p1(low_points);
  p2(low_points);

  return 0;
}
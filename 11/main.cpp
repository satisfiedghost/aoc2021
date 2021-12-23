#include "input.h"
#include <vector>
#include <iostream>
#include <functional>
#include <vector>

struct Octopus {
  Octopus() : m_energy(0), m_has_flashed(false) {}

  uint8_t m_energy;
  std::vector<std::reference_wrapper<Octopus>> m_neighbors;
  bool m_has_flashed;

  void flash() {
    m_has_flashed = true;
    for (auto& o : m_neighbors) {
      o.get().m_energy++;
    }
  }

  void reset() {
    m_has_flashed = false;
    m_energy = 0;
  }

  void add_neighbor(Octopus& o) {
    m_neighbors.push_back(o);
  }
};

std::ostream& operator<<(std::ostream& os, const Octopus& o) {
  os << static_cast<int>(o.m_energy) << " " << (o.m_has_flashed) ? "true" : "false";
  return os;
}

void try_add_neighbor(Octopus &octo, Octopus octo_grid[10][10], size_t i, size_t j) {
  // lazy - take advantage of underflow
  if (i > 9 || j > 9) {
    return;
  }
  octo.add_neighbor(octo_grid[i][j]);
}

int main() {
  Octopus octo_grid[10][10];
  std::vector<std::reference_wrapper<Octopus>> octo_vec;

  // initial energy levels
  for (size_t i = 0; i < 10; i++) {
    for (size_t j = 0; j < 10; j++) {
      octo_grid[i][j].m_energy = g_input[i][j];
      octo_vec.push_back(octo_grid[i][j]);
    }
  }

  // create neighbors
  for (size_t i = 0; i < 10; i++) {
    for (size_t j = 0; j < 10; j++) {
      // has 8 neighbors
      Octopus& cur = octo_grid[i][j];
      // top left
      try_add_neighbor(cur, octo_grid, i - 1, j - 1);
      // above
      try_add_neighbor(cur, octo_grid, i - 1, j);
      // top right
      try_add_neighbor(cur, octo_grid, i - 1, j + 1);
      // right
      try_add_neighbor(cur, octo_grid, i, j + 1);
      // bottom right
      try_add_neighbor(cur, octo_grid, i + 1, j + 1);
      // below
      try_add_neighbor(cur, octo_grid, i + 1, j);
      // bottom left
      try_add_neighbor(cur, octo_grid, i + 1, j - 1);
      // left
      try_add_neighbor(cur, octo_grid, i, j - 1);
    }
  }

  for (auto& o : octo_vec) {
      std::cout << "octopus " << o << std::endl;
      std::cout << "with neighbors " << std::endl;
      for (const auto& n : o.get().m_neighbors) {
        std::cout << n << std::endl;
      }
  }

  size_t num_flashes = 0;
  bool sync_step = false;
  size_t step = 0;
  // run 100 steps...
  while(!sync_step) {
    num_flashes = 0;
    // everyone gets +1 energy
    for (auto& o : octo_vec) {
      o.get().m_energy++;
    }

    // cascade until we're done...
    bool has_anyone_flashed = false;
    do {
      has_anyone_flashed = false;
      for (auto& o : octo_vec) {
        if (!o.get().m_has_flashed && o.get().m_energy >= 10) {
          o.get().flash();
          num_flashes++;
          has_anyone_flashed = true;
        }
      }
    } while(has_anyone_flashed);

    // reset flashers
    for (auto& o : octo_vec) {
      if (o.get().m_has_flashed) {
        o.get().reset();
      }
    }
    if (num_flashes == 100) {
      sync_step = true;
    }
    step++;
  }

  //std::cout << "P1: " << num_flashes << std::endl;
  std::cout << "P2: " << step << std::endl;

  return 0;
}
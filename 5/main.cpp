#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <tuple>
#include <memory>
#include <cstring>

// Grid size
static constexpr size_t N_ROWS = 1000UL;
static constexpr size_t N_COLS = 1000UL;

typedef struct ThermalLine {

  constexpr ThermalLine() : m_start(std::make_tuple(0, 0)), m_end(std::make_tuple(0, 0)) {}
  constexpr ThermalLine(size_t x1, size_t y1, size_t x2, size_t y2)
                        : m_start(std::make_tuple(x1, y1))
                        , m_end(std::make_tuple(x2, y2)) 
                        {}

  size_t x1() const { return std::get<0>(m_start); }
  size_t y1() const { return std::get<1>(m_start); }
  size_t x2() const { return std::get<0>(m_end); }
  size_t y2() const { return std::get<1>(m_end); }

  const std::tuple<size_t, size_t> first() const {return m_start;}
  const std::tuple<size_t, size_t> second() const {return m_end;}

private:
  // inclusive start/end coords
  std::tuple<size_t, size_t> m_start;
  std::tuple<size_t, size_t> m_end;

}ThermalLine;

// teach C++ to print this struct
std::ostream& operator<<(std::ostream& os, const ThermalLine &t) {
  os << "(" << std::get<0>(t.first()) << ", " << std::get<1>(t.first()) << ")" << "->"
  << "(" << std::get<0>(t.second()) << ", " << std::get<1>(t.second()) << ")";
  return os;
}

// I LOVE parsing strings in C++ I CANNOT get ENOUGH of this
std::unique_ptr<ThermalLine> create_thermal_from_str(const std::string &str) {
  size_t x1, y1, x2, y2;
  std::istringstream coords(str);
  std::string tmp;

  std::getline(coords, tmp, ',');
  x1 = std::atoi(tmp.c_str());
  std::getline(coords, tmp, ' ');
  y1 = std::atoi(tmp.c_str());

  // throw away '-> '
  std::getline(coords, tmp, ' ');

  std::getline(coords, tmp, ',');
  x2 = std::atoi(tmp.c_str());
  std::getline(coords, tmp, ' ');
  y2 = std::atoi(tmp.c_str());

  return std::make_unique<ThermalLine>(x1, y1, x2, y2);
}

size_t find_danger_zones(size_t grid[N_ROWS][N_COLS], size_t threshold) {
  size_t danger_zone_cnt = 0;
  for (size_t i = 0; i < N_ROWS; i++) {
    for (size_t j = 0; j < N_COLS; j++) {
      if (grid[i][j] >= threshold) {
        danger_zone_cnt++;
      }
    }
  }
  return danger_zone_cnt;
}

int main() {
  std::ifstream input("input.txt");
  std::string input_line;
  std::list<std::unique_ptr<ThermalLine>> thermal_lines;

  // create thermal lines
  while (std::getline(input, input_line)) {
    thermal_lines.push_back(std::move(create_thermal_from_str(input_line)));
  }

  // create ocean grid
  size_t ocean_floor[N_ROWS][N_COLS];
  for (size_t i = 0; i < N_ROWS; i++) {
    std::memset(ocean_floor[i], 0, sizeof(size_t) * N_COLS);
  }

  std::cout << "Number of overlapping zones: " << find_danger_zones(ocean_floor, 1) << std::endl;

  // connect the lines...
  // wow this is a monster and I'm sure there's a better way to handle this
  for (const std::unique_ptr<ThermalLine> &t : thermal_lines) {
    // accounting for this edge case makes our indexing easier later on (can assume some iteration is needed)
    if (t->first() == t->second()) {
      // row major access is kind of weird but ok
      ocean_floor[t->y1()][t->x1()]++;
    } else if (t->x1() == t->x2()) {
      const size_t xidx = t->x1();
      // we know these aren't equal but have no idea if they're ordered
      // use int to make for-loop convenient
      int yidx = t->y1();
      if (yidx < static_cast<int>(t->y2())) {
        for (; yidx <= static_cast<int>(t->y2()); yidx++) {
          ocean_floor[yidx][xidx]++;
        }
      } else {
        for (; yidx >= static_cast<int>(t->y2()); yidx--) {
          ocean_floor[yidx][xidx]++;
        }
      }
    } else if (t->y1() == t->y2()) {
      const size_t yidx = t->y1();
      // we know these aren't equal but have no idea if they're ordered
      // use int to make for-loop convenient
      int xidx = t->x1();
      if (xidx < static_cast<int>(t->x2())) {
        for (; xidx <= static_cast<int>(t->x2()); xidx++) {
          ocean_floor[yidx][xidx]++;
        }
      } else {
        for (; xidx >= static_cast<int>(t->x2()); xidx--) {
          ocean_floor[yidx][xidx]++;
        }
      }
    } else { // handle diagonal case
      int xidx = t->x1();
      int yidx = t->y1();
      while (xidx != static_cast<int>(t->x2()) and yidx != static_cast<int>(t->y2())) {
        ocean_floor[yidx][xidx]++;
        if (xidx < static_cast<int>(t->x2())) {
          xidx++;
        } else {
          xidx--;
        }
        if (yidx < static_cast<int>(t->y2())) {
          yidx++;
        } else {
          yidx--;
        }
      }
      ocean_floor[yidx][xidx]++;
    }
  }

  std::cout << "Number of overlapping zones: " << find_danger_zones(ocean_floor, 2) << std::endl;

  return 0;
}
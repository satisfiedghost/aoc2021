#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <tuple>
#include <memory>
#include <cstring>
#include <map>

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

std::ostream& operator<<(std::ostream& os, const std::tuple<size_t, size_t> &t) {
  os << "(" << std::get<0>(t) << "," << std::get<1>(t) << ")" << std::endl;
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

void find_points_in_line(std::list<std::tuple<size_t, size_t>>& list, const std::unique_ptr<ThermalLine>& tline) {
  list.clear();

  // line of length 1...?
  if (tline->first() == tline->second()) {
    list.push_back(tline->first());
    return;
  }

  size_t lowerx = tline->x1() < tline->x2() ? tline->x1() : tline->x2();
  size_t upperx = tline->x1() > tline->x2() ? tline->x1() : tline->x2();
  size_t lowery = tline->y1() < tline->y2() ? tline->y1() : tline->y2();
  size_t uppery = tline->y1() > tline->y2() ? tline->y1() : tline->y2();

  if (tline->y1() == tline->y2()) { // horizontal
    for (; lowerx <= upperx; lowerx++) {
      // lowery and uppery the same here.. just pick one
      list.push_back(std::make_tuple(lowerx, lowery));
    }
  } else if (tline->x1() == tline->x2()) { // vertical
    for (; lowery <= uppery; lowery++) {
      // lowerx and upperx the same here.. just pick one
      list.push_back(std::make_tuple(lowerx, lowery));
    }
  } else { // diagonal
    // positive or negative slope? (positive == truthy)
    bool slope = (tline->y1() < tline->y2()) ? true : false;
    // now go left to right
    std::tuple<size_t, size_t> current = 
      lowerx == tline->x1() ? tline->first() : tline->second();
    std::tuple<size_t, size_t> end = 
      lowerx == tline->x1() ? tline->second() : tline->first();

    std::cout << "current " << current << std::endl;
    std::cout << "end " << end << std::endl;

    while (current != end) {
      list.push_back(current);
      std::get<0>(current)++;
      if (slope) {
        std::get<1>(current)++;
      } else {
        std::get<1>(current)--;
      }
      std::cout << "now " << current <<std::endl;
    }
    list.push_back(end);
  }
}

int main() {
  std::ifstream input("input.txt");
  std::string input_line;
  std::list<std::unique_ptr<ThermalLine>> thermal_lines;

  // create thermal lines
  while (std::getline(input, input_line)) {
    thermal_lines.push_back(std::move(create_thermal_from_str(input_line)));
  }

  // frequency mapping
  std::map<std::tuple<size_t, size_t>, size_t> map;
  // points contained in a line
  std::list<std::tuple<size_t, size_t>> point_list;

  // connect the lines...
  for (const std::unique_ptr<ThermalLine> &t : thermal_lines) {
    find_points_in_line(point_list, t);
    for (auto p : point_list) {
      if (map.find(p) != map.end()) {
        map[p]++;
      } else {
        map.insert({p, 0});
      }
    }
  }

  size_t danger_zones = 0;
  for (auto const& p : map) {
    if (p.second >= 2) {
      danger_zones++;
    }
  }

  std::cout << "Dangers zones: " << danger_zones << std::endl;

  return 0;
}
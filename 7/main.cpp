#include "input.h"
#include <iostream>
#include <map>
#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>


//size_t find_mode(const std::list<size_t>& positions) {
//  std::map<size_t, size_t> map;
//  for (const auto& p : positions) {
//    if (map.find(p) == map.end()) {
//      map.insert({p, 1});
//    } else {
//      map[p]++;
//    }
//  }
//
//  // find mode
//  size_t mode = 0;
//  size_t occurances = 0;
//  for (const auto& p : map) {
//    std::cout << "{" << p.first << " : " << p.second << "}" << std::endl;
//    if (p.second > occurances) {
//      mode = p.first;
//      occurances = p.second;
//    }
//  }
//
//  return mode;
//}
//
//size_t find_avg(const std::list<size_t>& positions) {
//  size_t sum = 0;
//  for (const auto& p : positions) {
//    sum += p;
//  }
//  return sum / positions.size();
//}

int main() {
  // brute force baby
  size_t fuel = std::numeric_limits<size_t>::max();
  for (const auto& a : g_input) {
    size_t this_run = 0;
    for (const auto& b : g_input) {
      this_run += std::abs(static_cast<long int>(a) - static_cast<long int>(b));
    }
    if (this_run < fuel) {
      fuel = this_run;
    }
  }

  std::cout << "fuel p1: " << fuel << std::endl;

  // oh yeah that's the good stuff
  fuel = std::numeric_limits<size_t>::max();
  for (size_t i = 0; i < *std::max_element(g_input.begin(), g_input.end()); i++) {
    size_t this_run = 0;
    for (const auto& test: g_input) {
      size_t distance = std::abs(static_cast<long int>(i) - static_cast<long int>(test));
      size_t cost_to_move = 0;
      for (size_t i = 0; i <= distance; i++) {
        cost_to_move += i;
      }
      this_run += cost_to_move;
    }
    if (this_run < fuel) {
      fuel = this_run;
    }
  }

  std::cout << "fuel p2: " << fuel << std::endl;

  return 0;
}
#include <iostream>
#include <fstream>
#include "cmds.h"
#include <tuple>

int main() {
  // horizontal, depth, aim
  std::tuple<int, int, int> coords = std::make_tuple(0, 0, 0);

  for (cmd c : g_cmds) {
    if (c.mov == "forward") {
      std::get<0>(coords) = std::get<0>(coords) + c.value;
      std::get<1>(coords) = std::get<1>(coords) + std::get<2>(coords) * c.value;
    } else if (c.mov == "down") {
      std::get<2>(coords) = std::get<2>(coords) + c.value;
    } else if (c.mov == "up") {
      std::get<2>(coords) = std::get<2>(coords) - c.value;
    }
  }

  std::cout << std::get<0>(coords) << " " << std::get<1>(coords) << std::endl;
  std::cout << std::get<0>(coords) *
               std::get<1>(coords) << std::endl;

  return 0;
}
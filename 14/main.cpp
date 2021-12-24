#include "input.h"

#include <iostream>
#include <vector>
#include <limits>
using std::vector;

template<typename T>
std::ostream& operator<<(std::ostream& os, const vector<char> &vec) {
  for (const auto& c : vec) {
    os << c;
  }
  os << std::endl;
  return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const map<T, size_t> &map) {
  for (const auto& entry : map) {
    os << "{" << entry.first << ":" << entry.second << "}" << std::endl;
  }
  os << std::endl;
  return os;
}

template<typename T>
void try_increment_element(map<T, size_t>& ele_count, T c, size_t i = 1) {
    if (ele_count.find(c) == ele_count.end()) {
      ele_count.insert({c, i});
    } else {
      ele_count[c] += i;
    }
}

void apply_step(map<string, size_t>& pairs, map<char, size_t>& ele_count) {
  // ugly, copy the map to avoid complications of modifying in-place
  map<string, size_t> pair_copy = pairs;
  for (auto& p : pair_copy) {
    // this pair may not exist anymore!
    if (pair_copy[p.first] == 0) {
      continue;
    }
    // each pair will result in that particular pair being destroyed, as well as a new element and 2 new pairs
    // do this for the # of pairs
    char new_element = g_rules[p.first];
    size_t occurance_count = p.second;
    //for (size_t i = 0; i < p.second; i++) {


    try_increment_element(ele_count, new_element, occurance_count);
  
    // add the new elements
    string new_pair1{p.first[0], new_element};
    string new_pair2{new_element, p.first[1]};

    try_increment_element(pairs, new_pair1, occurance_count);
    try_increment_element(pairs, new_pair2, occurance_count);

    // and remove the "broken" link
    pairs[p.first] -= occurance_count;
    //}
  }
  std::cout << pairs;
  std::cout << ele_count;
}

int main() {
  map<string, size_t> pairs;
  map<char, size_t> ele_count;

  for (auto c : g_input) {
    try_increment_element(ele_count, c);
  }

  size_t idx = 0;
  while (idx < g_input.size() - 1) {
    string pair{g_input[idx], g_input[idx + 1]};
    try_increment_element(pairs, pair);
    idx++;
  } 

  for (size_t i = 0; i < 40; i++) {
    std::cout << "step " << i + 1 << std::endl;
    apply_step(pairs, ele_count);
  }

  size_t max = 0, min = std::numeric_limits<size_t>::max();

  for (const auto& e : ele_count) {
    if (e.second > max) {
      max = e.second;
    }
    if (e.second < min) {
      min = e.second;
    }
  }

  std::cout << "P2 " << max - min << std::endl;


  return 0;
}
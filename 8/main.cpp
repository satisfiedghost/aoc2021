#include "input.h"
#include <iostream>
#include <set>
#include <memory>
#include <algorithm>
#include <map>
#include <boost/bimap.hpp>

void p1() {
  size_t cnt = 0;
  for (const auto& line : g_input) {
    auto out = std::get<1>(line);
    for (const auto& o : out) {
      if (o.size() == 2 || o.size() == 3 || o.size() == 4 || o.size() == 7) {
        cnt++;
      }
    }
  }
  std::cout << "P1: " << cnt << std::endl;
}

void clear_and_populate_set(std::set<char>& set, const std::string& str) {
  set.clear();
  for (char c : str) {
    set.insert(c);
  }
}

template<typename T>
void set_difference(const std::set<T>& a, const std::set<T>& b, std::set<T>& diff) {
  diff.clear();
  std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::inserter(diff, diff.begin()));
}

// useful for debug...
template<typename T>
void set_union(const std::set<T>& a, const std::set<T>& b, std::set<T>& unio) {
  unio.clear();
  std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::inserter(unio, unio.begin()));
}

// useful for debug...
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T> &set) {
  for (const auto& s : set) {
    os << s << " ";
  }
  return os;
}

static size_t s_total = 0;

void decode_outputs(const boost::bimap<size_t, char>::right_map& map, std::list<std::string> outputs) {
  // check out each string...
  const std::set<size_t> zero = {0, 1, 2, 4, 5, 6};
  const std::set<size_t> one = {2, 5};
  const std::set<size_t> two = {0, 2, 3, 4, 6};
  const std::set<size_t> three = {0, 2, 3, 5, 6};
  const std::set<size_t> four = {1, 2, 3, 5};
  const std::set<size_t> five = {0, 1, 3, 5, 6};
  const std::set<size_t> six = {0, 1, 3, 4, 5, 6};
  const std::set<size_t> seven = {0, 2, 5};
  const std::set<size_t> eight = {0, 1, 2, 3, 4, 5, 6};
  const std::set<size_t> nine = {0, 1, 2, 3, 5, 6};

  // convenient access
  const std::array<const std::set<size_t>, 10> list = {
    zero, one, two, three, four, five, six, seven, eight, nine
  };

  std::set<size_t> digit;
  size_t value = 0;
  for(const auto& s : outputs) {
    // look at each character...
    for (auto c : s) {
      digit.insert(map.find(c)->second);
    }

    for (size_t i = 0; i < list.size(); i++) {
      // decoded
      if (digit == list[i]) {
        value *= 10;
        value += i;
      }
    }
    digit.clear();
  }
  s_total += value;

  std::cout << "Decoded " << value << std::endl;
  std::cout << "Running Total: " << s_total << std::endl;
}



void p2() {
  std::set<char> one;
  std::set<char> two;
  std::set<char> three;
  std::set<char> four;
  std::set<char> five;
  std::set<char> six;
  std::set<char> seven;
  std::set<char> eight;
  std::set<char> nine;
  std::set<char> zero;
  std::list<std::set<char>> six_digits;
  std::list<std::set<char>> five_digits;
  // mtaintain this convention
  /*
        0000
      1      2
      1      2
        3333
      4      5
      4      5
        6666
  */ 


  // it's convenient to build the map this way, and reverse it later
  boost::bimap<size_t, char> wire_bimap;
  boost::bimap<size_t, char>::left_map wire_map = wire_bimap.left;

  for (const auto & line : g_input) {
    wire_bimap.clear();
    // look at all input sequences, which we know correspond to unique digits somehow...
    // we can construct some initial sets, knowing that some digits have a unique count
    for (const auto& s : std::get<0>(line)) {
      if (s.size() == 2) { // one
        clear_and_populate_set(one, s);
      } else if (s.size() == 4) { // four
        clear_and_populate_set(four, s);
      } else if (s.size() == 3) { // seven
        clear_and_populate_set(seven, s);
      } else if (s.size() == 7) { // eight
        clear_and_populate_set(eight, s);
      } else if (s.size() == 6) {
        std::set<char> six_digit;
        clear_and_populate_set(six_digit, s);
        six_digits.push_back(six_digit);
      } else if (s.size() == 5) {
        std::set<char> five_digit;
        clear_and_populate_set(five_digit, s);
        five_digits.push_back(five_digit);
      }
    }

    // now the fun part
    std::set<char> diff;
    std::set<char> unio; // union is a keyword...
    set_difference(seven, one, diff);
    // should be size 1 sets
    wire_map.insert({0, *diff.begin()});

    // find top right '2'
    for (const auto& s : six_digits) {
      set_union(s, one, unio);
      // if this forms the whole set, then we have merged
      // 6 and one and the diff of 8 and 6 is the top right mapping
      if (unio.size() == 7) {
        set_difference(eight, s, diff);
        six = s;
        wire_map.insert({2, *diff.begin()});
        break;
      }
    }

    // bottom right
    set_difference(one, {wire_map.find(2)->second}, diff);
    wire_map.insert({5, *diff.begin()});

    // identify 9
    for (const auto& s : six_digits) {
      set_union(four, s, unio);
      if (unio.size() < 7) {
        nine = s;
        break;
      }
    }

    // bottom left
    set_difference(eight, nine, diff);
    wire_map.insert({4, *diff.begin()});
    
    // now we just need '3' and '6'
    // identify 0
    for (const auto& s : six_digits) {
      if (s != six && s != nine) {
        zero = s;
        break;
      }
    }
    // middle
    set_difference(eight, zero, diff);
    wire_map.insert({3, *diff.begin()});

    // top left mapping
    set_difference(four, one, diff);
    // remove middle
    std::set<char> idontcareanymore = diff;
    set_difference(idontcareanymore, {wire_map.find(3)->second}, diff);
    wire_map.insert({1, *diff.begin()});

    // and who's left?
    set_difference(eight, {wire_map.find(0)->second,
                           wire_map.find(1)->second, 
                           wire_map.find(2)->second, 
                           wire_map.find(3)->second, 
                           wire_map.find(4)->second, 
                           wire_map.find(5)->second}, diff);

    wire_map.insert({6, *diff.begin()});
    decode_outputs(wire_bimap.right, std::get<1>(line));
    five_digits.clear();
    six_digits.clear();
  }
}

int main() {
  p1();
  p2();

  return 0;
}
#include "input.h"
#include <map>
#include <iostream>
#include <set>
#include <algorithm>

// returns iterator to first element grater than limit
std::set<std::tuple<size_t, size_t>>::iterator upper_bound(std::set<std::tuple<size_t, size_t>> &s, size_t limit) {

  auto it = s.begin();
  while (std::get<0>(*it) < limit && it != s.end()) {
    it++;
  }

  return it;
}

// returns a reverse iterator to the first element less than limit
std::set<std::tuple<size_t, size_t>>::reverse_iterator lower_bound(std::set<std::tuple<size_t, size_t>> &s, size_t limit) {

  auto it = s.rbegin();
  while (std::get<0>(*it) > limit && it != s.rend()) {
    it++;
  }

  return it;
}

template<typename T>
void set_union(const std::set<T>& a, const std::set<T>& b, std::set<T>& unio) {
  unio.clear();
  std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::inserter(unio, unio.begin()));
}

void p1(std::set<std::tuple<size_t, size_t>> dots_by_x, std::set<std::tuple<size_t, size_t>> dots_by_y) {
  // fold at x = 655
  // first we get the set of all dots with x > 655
  auto it = upper_bound(dots_by_x, 655);

  // create the mirrored right-hand set
  std::set<std::tuple<size_t, size_t>> mirror_set;
  while (it != dots_by_x.end()) {
    size_t dist_from_fold = std::get<0>(*it) - 655;
    mirror_set.insert({655 - dist_from_fold, std::get<1>(*it)});
    it++;
  }

  auto rit = lower_bound(dots_by_x, 655);

  std::set<std::tuple<size_t, size_t>> left_set;
  while (rit != dots_by_x.rend()) {
    left_set.insert(*rit);
    rit++;
  }

  std::set<std::tuple<size_t, size_t>> overlap;
  set_union(left_set, mirror_set, overlap);

  std::cout << "P1: " << overlap.size() << std::endl;
}

void redo_sets(std::set<std::tuple<size_t, size_t>>& dotx, std::set<std::tuple<size_t, size_t>>& doty, std::set<std::tuple<size_t, size_t>>& overlap) {

  dotx.clear();
  doty.clear();

  for (auto& i : overlap) {
    dotx.insert(i);
    doty.insert(std::make_tuple(std::get<1>(i), std::get<0>(i)));
  }
}

void xfold(std::set<std::tuple<size_t, size_t>>& dotx, std::set<std::tuple<size_t, size_t>>& doty, size_t fold) {
  // first we get the set of all dots with x > fold
  auto it = upper_bound(dotx, fold);

  // create the mirrored right-hand set
  std::set<std::tuple<size_t, size_t>> mirror_set;
  while (it != dotx.end()) {
    size_t dist_from_fold = std::get<0>(*it) - fold;
    mirror_set.insert({fold - dist_from_fold, std::get<1>(*it)});
    it++;
  }

  // get all dots remaining on the left side...
  auto rit = lower_bound(dotx, fold);
  std::set<std::tuple<size_t, size_t>> left_set;
  while (rit != dotx.rend()) {
    left_set.insert(*rit);
    rit++;
  }
  std::set<std::tuple<size_t, size_t>> overlap;
  set_union(left_set, mirror_set, overlap);

  // now we effectively have a new grid and our overlap set contains all those points
  redo_sets(dotx, doty, overlap);
}

void yfold(std::set<std::tuple<size_t, size_t>>& dotx, std::set<std::tuple<size_t, size_t>>& doty, size_t fold) {
  // first we get the set of all dots with y > fold
  auto it = upper_bound(doty, fold);

  // create the mirrored bottom set
  std::set<std::tuple<size_t, size_t>> mirror_set;
  while (it != doty.end()) {
    size_t dist_from_fold = std::get<0>(*it) - fold;
    mirror_set.insert({fold - dist_from_fold, std::get<1>(*it)});
    it++;
  }

  // get all dots remaining on the top side...
  auto rit = lower_bound(doty, fold);
  std::set<std::tuple<size_t, size_t>> top_set;
  while (rit != doty.rend()) {
    top_set.insert(*rit);
    rit++;
  }
  std::set<std::tuple<size_t, size_t>> overlap;
  set_union(top_set, mirror_set, overlap);

  // now we effectively have a new grid and our overlap set contains all those points
  // remember we have to swap these this time
  redo_sets(doty, dotx, overlap);
}

void p2(std::set<std::tuple<size_t, size_t>>& dotx, std::set<std::tuple<size_t, size_t>>& doty) {
  xfold(dotx, doty, 655);
  yfold(dotx, doty, 447);
  xfold(dotx, doty, 327);
  yfold(dotx, doty, 223);
  xfold(dotx, doty, 163);
  yfold(dotx, doty, 111);
  xfold(dotx, doty, 81);
  yfold(dotx, doty, 55);
  xfold(dotx, doty, 40);
  yfold(dotx, doty, 27);
  yfold(dotx, doty, 13);
  yfold(dotx, doty, 6);

  std::cout << "P2" << std::endl;

  char block[20][100];
  for (size_t i = 0; i < 20; i++) {
    for (size_t j = 0; j < 100; j++) {
      block[i][j] = ' ';
    }
  }


  for (const auto& d : doty) {
    block[std::get<0>(d)][std::get<1>(d)] = '#';
  }

  for (size_t i = 0; i < 20; i++) {
    for (size_t j = 0; j < 100; j++) {
      std::cout << block[i][j];
    }
    std::cout << std::endl;
  }
}


int main() {
  std::set<std::tuple<size_t, size_t>> dots_by_x;
  std::set<std::tuple<size_t, size_t>> dots_by_y;

  for (auto& i : g_input) {
    dots_by_x.insert(i);
    dots_by_y.insert(std::make_tuple(std::get<1>(i), std::get<0>(i)));
  }

  p1(dots_by_x, dots_by_y);
  p2(dots_by_x, dots_by_y);


  return 0;
}
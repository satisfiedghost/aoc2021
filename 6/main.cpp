#include "input.h"
#include <map>
#include <iostream>

int main(int argc, char** argv) {
  size_t days = 0;

  if (argc < 2) {
    std::cout << "Calculating boring case..." << std::endl;
  } else if (argc > 3) {
    std::cout << "Usage: ./a.out [# fish]" << std::endl;
    exit(1);
  } else {
    days = std::atoi(argv[1]);
  }

  std::map<uint8_t, size_t> producers;
  size_t input_size = sizeof(g_input) / sizeof(g_input[0]);

  // fish can have these timers
  for (uint8_t i = 0; i <= 8; i++) {
    producers.insert({i, 0});
  }

  // construct initial conditions
  for (size_t i = 0; i < input_size; i++) {
    producers[g_input[i]]++;
  }

  // run simulation
  for (size_t i = 0; i < days; i++) {
    // needs to save fish in group 1 temporarily
    size_t grp1 = producers[1];

    // every fish's timer decreases by 1
    // move those fish down a bucket
    for (uint8_t j = 1; j < 8; j++) {
      producers[j] = producers[j + 1];
    }

    // fish in group 0 get cycled back up to group 6
    producers[6] += producers[0];

    // each fish in group 0 produces a brand new fish with a timer of 8
    producers[8] = producers[0];

    // move group 1 to 0
    producers[0] = grp1;
  }

  // you've heard of "some pig"
  // now get ready for "sum fish"
  size_t sum_fish = 0;

  // count up all these god-damned fish
  for (uint8_t i = 0; i <= 8; i++) {
    sum_fish += producers[i];
  }

  std::cout << "Wow there are " << sum_fish <<  " fish here." << std::endl;

  return 0;
}
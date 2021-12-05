#include <iostream>
#include <fstream>
#include <string>
#include <list>


// hardcode because we know the input size...
static constexpr size_t N_COLS = 12;
static constexpr size_t N_ROWS = 1000;

static char s_matrix[N_ROWS][N_COLS];

void process_line(const std::string &line) {
  size_t len = line.length();
  // oh yeah, we're naughty this year
  static size_t row = 0;

  for (size_t i = 0; i < len; i++) {
    s_matrix[row][i] = line[i];
  }
  row++;
}

static uint16_t common_to_uint16_t(const std::size_t s[]) {
  // bit-bang to form 12-bit number
  uint16_t n = 0;
  for (size_t i = 0; i < N_COLS; i++) {
    n <<= 1;
    if (s[i] > 500) {
      n |= 1;
    }
  }
  return n;
}

uint16_t calculate_gamma() {
  // cnts of 1s occuring in this column
  std::size_t one_cnt[N_COLS] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  for (size_t i = 0; i < N_ROWS; i++) {
    for (size_t j = 0; j < N_COLS; j++) {
      if (s_matrix[i][j] == '1') {
        one_cnt[j]++;
      }
    }
  }

  return common_to_uint16_t(one_cnt);
}

static uint16_t bin_to_uint16_t(const char* bin_str) {
  uint16_t n = 0;
  for (size_t i = 0; i < N_COLS; i++) {
    n <<= 1;
    if (bin_str[i] == '1') {
      n |= 1;
    }
  }
  return n;
}

uint16_t calculate_o2(const std::size_t idx, const std::list<const char*> &values) {
  // we're done here
  if (values.size() == 1) {
    return bin_to_uint16_t(values.front());
  }

  // the values we have left, with ones or zeros in this idx
  std::list<const char*> ones;
  std::list<const char*> zeros;

  for (auto v : values) {
    if (v[idx] == '1') {
      ones.push_back(v);
    } else {
      zeros.push_back(v);
    }
  }

  if (ones.size() >= zeros.size()) {
    return calculate_o2(idx + 1, ones);
  } else {
    return calculate_o2(idx + 1, zeros);
  }
}

// TODO make this and the above generic...
uint16_t calculate_co2(const std::size_t idx, const std::list<const char*> &values) {
  // we're done here
  if (values.size() == 1) {
    return bin_to_uint16_t(values.front());
  }

  // the values we have left, with ones or zeros in this idx
  std::list<const char*> ones;
  std::list<const char*> zeros;

  for (auto v : values) {
    if (v[idx] == '1') {
      ones.push_back(v);
    } else {
      zeros.push_back(v);
    }
  }

  if (ones.size() < zeros.size()) {
    return calculate_co2(idx + 1, ones);
  } else {
    return (calculate_co2(idx + 1, zeros));
  }

}

int main() {
  std::ifstream input("input.txt");

  std::string s;
  while (std::getline(input, s)) {
    process_line(s);
  }

  uint16_t gamma = calculate_gamma();
  uint16_t epsilon = (gamma ^ 0xFFFF) & 0x0FFF;

  // should've just had a list to begin with....
  std::list<const char*> values;
  for (size_t i = 0; i < N_ROWS; i++) {
    values.push_back(s_matrix[i]);
  }

  uint16_t o2 = calculate_o2(0, values);
  uint16_t co2 = calculate_co2(0, values);

  std::cout << "gamma " << gamma << std::endl;
  std::cout << "epsilon " << epsilon << std::endl;
  std::cout << "gamma * epsilon " << gamma * epsilon << std::endl;

  std::cout << "o2 " << o2 << std::endl;
  std::cout << "co2 " << co2 << std::endl;
  std::cout << "o2 * co2 " << o2 * co2 << std::endl;

  return 0;
}
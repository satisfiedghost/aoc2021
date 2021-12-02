#include <iostream>
#include <fstream>
#include <limits>

int main() {

  std::fstream file("input.txt", std::ios_base::in);

  size_t incr = 0;
  long int a = std::numeric_limits<int>::max(),
           b = std::numeric_limits<int>::max(),
           c = std::numeric_limits<int>::max(),
           d = std::numeric_limits<int>::max();

  do {
    if (a + b + c < b + c + d) {
      incr++;
    }
    std::cout << std::endl;
    a = b;
    b = c;
    c = d;
    
  } while (file >> d);

  std::cout << incr << std::endl;

  return 0;
}
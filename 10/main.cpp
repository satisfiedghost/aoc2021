#include <iostream>
#include <stack>
#include <fstream>
#include <string>
#include <set>
#include <unordered_map>
#include <list>

const char square_end = ']';
const char angle_end = '>';
const char curly_end = '}';
const char paren_end = ')';


const std::set<char> ends = {square_end, angle_end, curly_end, paren_end};
std::unordered_map<char, size_t> scores = {
  {square_end, 57UL},
  {angle_end, 25137UL},
  {curly_end, 1197UL},
  {paren_end, 3UL}
};

void fp2(std::stack<char>& s, std::list<size_t>& p2) {
  const size_t size = s.size();
  size_t score = 0;

  for (size_t i = 0; i < size; i++) {
    char c = s.top();
    s.pop();

    score *= 5;
    switch(c) {
      case '(':
        score += 1;
      break;
      case '[':
        score += 2;
      break;
      case '{':
        score += 3;
      break;
      case '<':
        score += 4;
      break;
    }
  }
  std::cout << "pushing " << score << std::endl;
  p2.push_back(score);
}

size_t p1(const std::string& str, std::list<size_t>& p2) {
  std::stack<char> stack;

  for (size_t i = 0; i < str.size(); i++) {
    const char c = str[i];
    // found a terminating block char
    if (ends.find(c) != ends.end()) {
      // this has no beginning mate!
      if (stack.size() == 0) {
        return scores[c];
      }
      char begin = stack.top();
      stack.pop();

      if (c == square_end && begin != '[') {
        return scores[c];
      }
      if (c == angle_end && begin != '<') {
        return scores[c];
      }
      if (c == curly_end && begin != '{') {
        return scores[c];
      }
      if (c == paren_end && begin != '(') {
        return scores[c];

      }

    } else { // opening char
      stack.push(c);
    }
  }

  // corrupted lines
  if (stack.size() != 0) {
    fp2(stack, p2);
  }
}

int main() {
  std::fstream input_file("input.txt");
  std::string line;
  size_t score = 0;
  std::list<size_t> p2;

  while (std::getline(input_file, line)) {
    size_t s = p1(line, p2);
    score += s;
    std::cout << s << std::endl;
  }

  std::cout << "Total Score: " << score << std::endl;
  p2.sort();

  std::list<size_t>::iterator it = p2.begin();
  std::advance(it, std::distance(p2.begin(), p2.end()) / 2);

  std::cout << "P2 middle score: " << *it << std::endl;

  return 0;
}
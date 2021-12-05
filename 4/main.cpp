#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <string>
#include <list>

constexpr bool APPEASE_SQUID = true;

class BingoCard {
public:
    BingoCard(uint32_t **v) : m_is_in_play(true) {
      for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) {
          m_cells[i][j] = Cell(v[i][j]);
        }
      }
    }

  void play_number(uint32_t n) {
    for (size_t i = 0; i < 5; i++) {
      for (size_t j = 0; j < 5; j++) {
        if (m_cells[i][j].m_value == n) {
          m_cells[i][j].m_played = true;
          // assume unique cells
          break;
        }
      }
    }
  }
  // true if this card has a winning row
  bool is_winner() {
    // check rows
    for (size_t i = 0; i < 5; i++) {
      if (m_cells[i][0].m_played &&
          m_cells[i][1].m_played &&
          m_cells[i][2].m_played &&
          m_cells[i][3].m_played &&
          m_cells[i][4].m_played) {
            m_is_in_play = false;
            return true;
      }
    }
    // abuse contiguous memory (check columns)
    for (size_t i = 0; i < 5; i++) {
      if (m_cells[0][i].m_played &&
          m_cells[1][i].m_played &&
          m_cells[2][i].m_played &&
          m_cells[3][i].m_played &&
          m_cells[4][i].m_played) {
            m_is_in_play = false;
            return true;
      }
    }

    return false;
  }

  uint32_t sum_unplayed() const {
    uint32_t sum = 0;
    for (size_t i = 0; i < 5; i++) {
      for (size_t j = 0; j < 5; j++) {
        sum = m_cells[i][j].m_played ? sum : m_cells[i][j].m_value + sum;
      }
    }
    return sum;
  }

  void print() const {
    for (size_t i = 0; i < 5; i++) {
      for (size_t j = 0; j < 5; j++) {
        char played = m_cells[i][j].m_played ? 'P' : 'X';
        std::cout << m_cells[i][j].m_value << played << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  bool is_in_play() const {
    return m_is_in_play;
  }

private:
    struct Cell {
      uint32_t m_value;
      bool m_played;
      Cell() : m_value(0), m_played(false) {}
      Cell(uint32_t value) : m_value(value), m_played(false) {}
    };
    Cell m_cells[5][5];
    // Set to false once we determine this is a winner
    bool m_is_in_play;
};

int main() {
  std::ifstream input("input.txt");
  std::string line;

  // numbers called
  std::list<uint32_t> calls;

  // truly this is C++'s single weakness
  std::getline(input, line);
  std::istringstream call_stream(line);
  std::string call;
  while (getline(call_stream, call, ',')) {
    calls.push_back(std::atoi(call.c_str()));
  }

  // throw away spacer line
  getline(input, line);

  uint32_t **card_values = static_cast<uint32_t**>(malloc(sizeof(uint32_t *) * 5));
  for (size_t i = 0; i < 5; i++) {
    card_values[i] = static_cast<uint32_t*>(malloc(sizeof(uint32_t) * 5));
  }

  std::string value;
  std::istringstream value_row;

  // bingo cards
  std::list<BingoCard> bingo_cards;

  do {
    // create each card....
    for (size_t i = 0; i < 5; i++) {
      getline(input, line);
      value_row = std::istringstream(line);

      for (size_t j = 0; j < 5; j++) {
        getline(value_row, value, ' ');
        // hack to get around double spaces due to pretty-print input, how is this not easier in C++
        if (value == "") {
          getline(value_row, value, ' ');
        }
        card_values[i][j] = std::atoi(value.c_str());
      }
    }
    bingo_cards.push_back(BingoCard(card_values));

  } while (getline(input, line));

  // wow I do sure love managing my owm memory, nothing better
  for (size_t i = 0; i < 5; i++) {
    free(card_values[i]);
  }
  free(card_values);


  // play calls until we have a winner...
  for (auto call : calls) {
    // play for each card...
    std::cout << "calling... " << call << std::endl;
    for (auto &card : bingo_cards) {
      card.play_number(call);
    }

    for (auto &card : bingo_cards) {
      if (card.is_in_play() && card.is_winner()) {
        std::cout << std::endl << "winner!" << std::endl;
        card.print();
        std::cout << "unplayed sum: " << card.sum_unplayed() << std::endl;
        std::cout << "multiplied by winning call: " << card.sum_unplayed() * call << std::endl << std::endl;
        // TODO spend 2 hours refactoring this nested loop to avoid a perfectly reasonable use of goto on principle
        if (!APPEASE_SQUID) {
          goto gameover;
        }
      }
    }
  }
gameover:

  return 0;
}
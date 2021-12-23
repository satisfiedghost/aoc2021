#include "matrix.h"

void AdjMatrix::add_edge(std::string a, std::string b) {
  // any new nodes?
  if (str_to_idx.find(a) == str_to_idx.end()) {
    str_to_idx.insert({a, matrix_size});
    idx_to_str.insert({matrix_size, a});
    matrix_size++;
  }
  if (str_to_idx.find(b) == str_to_idx.end()) {
    str_to_idx.insert({b, matrix_size});
    idx_to_str.insert({matrix_size, b});
    matrix_size++;
  }
  // mark start and end nodes...
  if (a == "start") {
    start_idx = str_to_idx[a];
  } else if (b == "start") {
    start_idx = str_to_idx[b];
  }
  if (a == "end") {
    end_idx = str_to_idx[a];
  } else if (b == "end") {
    end_idx = str_to_idx[b];
  }

  // resize matrix if necessary
  // this is really inefficient, in a bigger dataset you'd want some heuristic method
  // of allocating more memory less often
  if (matrix_size > matrix.size()) {
    matrix.resize(matrix_size);
    for (auto& m : matrix) {
      m.resize(matrix_size, false);
    }
  }
  // we're guaranteed the above nodes are in the matrix bounds now
  matrix[str_to_idx[a]][str_to_idx[b]] = true;
  // this is an undirected graph
  matrix[str_to_idx[b]][str_to_idx[a]] = true;
}

const std::vector<std::vector<bool>>& AdjMatrix::get_readonly_matrix() const {
  return matrix;
}


const std::vector<bool>& AdjMatrix::operator[](size_t i) const {
  return matrix[i];
}

const std::string& AdjMatrix::get_name(size_t i) const {
  // at() has a const ref overload...
  // god if only someone would design a language with strict opt-in mutability
  return idx_to_str.at(i);
}

const size_t AdjMatrix::size() const {
  return matrix_size;
}

const bool AdjMatrix::is_small(size_t i) const {
  const std::string& s = idx_to_str.at(i);
  return std::islower(s[0]);
}

std::ostream& operator<<(std::ostream& os, const AdjMatrix& matrix) {
  const auto& m = matrix.get_readonly_matrix();

  os << std::left << std::setw(7) << " ";
  // just print the names first
  for (size_t i = 0; i < matrix.size(); i++) {
    os << std::left << std::setw(7) << " " + matrix.get_name(i);
  }
  os << std::endl;

  auto ph = [&](const AdjMatrix& matrix, std::ostream& os) {
    os << std::endl;
    os << std::setw(7) << " ";
    for (size_t i = 0; i < 7 * matrix.size(); i++) {
      os << "_";
    }
    os << std::endl;
  };

  ph(matrix, os);

  size_t idx = 0;
  for (const auto& l : m) {
    os << std::left << std::setw(7) << matrix.get_name(idx);
    for (const auto& e : l) {
      if (e) {
        os << std::left << std::setw(7) << "| X |";
      } else {
        os << std::left << std::setw(7) << "|   |";
      }

    }
    idx++;
    ph(matrix, os);
  }
  os << std::endl;
}
#include <unordered_map>
#include <vector>
#include <iomanip>

class AdjMatrix {
public:
  // add an edge
  void add_edge(std::string a, std::string b);

  // get a readonly copy of the internal adjacency matrix
  const std::vector<std::vector<bool>>& get_readonly_matrix() const;

  // overload for convenient access (now you can treat nodes as indices)
  const std::vector<bool>& operator[](size_t i) const;

  // get the name of a node by its index
  const std::string& get_name(size_t i) const;

  // if the adjacency matrix is NxN, this returns N
  const size_t size() const;

  // get start idx
  const size_t get_start_idx() const {return start_idx;};

  // get end idx
  const size_t get_end_idx() const {return end_idx;};

  // check if this is a small boring and pathetic cave
  const bool is_small(size_t i) const;

private:
  size_t matrix_size = 0;
  size_t start_idx = 0;
  size_t end_idx = 0;
  // switch to boost::bimap? 
  std::unordered_map<std::string, size_t> str_to_idx;
  std::unordered_map<size_t, std::string> idx_to_str;
  // true if there is an edge between these nodes
  std::vector<std::vector<bool>> matrix;
};

// cout this bad boy
std::ostream& operator<<(std::ostream& os, const AdjMatrix& matrix);
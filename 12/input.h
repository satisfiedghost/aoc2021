#include <string>
#include <tuple>
#include <list>

constexpr size_t ROWS = 22;
constexpr size_t COLS = 2;

std::list<std::tuple<std::string, std::string>> g_input = 
{{"ln", "nr"},
{"ln", "wy"},
{"fl", "XI"},
{"qc", "start"},
{"qq", "wy"},
{"qc", "ln"},
{"ZD", "nr"},
{"qc", "YN"},
{"XI", "wy"},
{"ln", "qq"},
{"ln", "XI"},
{"YN", "start"},
{"qq", "XI"},
{"nr", "XI"},
{"start", "qq"},
{"qq", "qc"},
{"end", "XI"},
{"qq", "YN"},
{"ln", "YN"},
{"end", "wy"},
{"qc", "nr"},
{"end", "nr"}};

#include "nuo/maybe.hpp"
#include "nuo/vec.hpp"
#include <iostream>

#define STRINGIFY(a) str_val(a)

#define str_val(a) #a

#define ASSERT(x)                                                              \
  std::cout << "   " << ((x) ? "\e[0;32m" : "\e[1;31m")                        \
            << ((x) ? "true" : "false") << "   "                               \
            << "\e[0m" << STRINGIFY(x) << "\n";

#define GROUP(name)                                                            \
  std::cout << "\e[0m"                                                         \
            << "Test Group :: "                                                \
            << "\e[1;33m" << name << "\n";

int main() {
  using nuo::Maybe;
  using nuo::Vec;
  auto vec1 = Vec<int>({50, 60});
  auto opt1 = Maybe<int>();

  GROUP("Vector")
  ASSERT(vec1[0] == 50) ASSERT(vec1[1] == 60) vec1.push(324);
  ASSERT(vec1[2] == 324) vec1.push(5434);
  ASSERT(vec1[3] == 5434)
  ASSERT(vec1.length() == 4)

  GROUP("Maybe")
  ASSERT(opt1.has() == false)
  opt1 = 5;
  ASSERT(opt1.has() == true)

  /* Tests complete */
  return 0;
}
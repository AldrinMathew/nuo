#include "nuo/maybe.hpp"
#include "nuo/vague.hpp"
#include "nuo/vec.hpp"
#include <iostream>

#define STRINGIFY(a) str_val(a)

#define str_val(a) #a

#define ASSERT(x)                                                              \
  std::cout << "   " << ((x) ? "\e[0;32m" : "\e[1;31m")                        \
            << ((x) ? "success" : "failure") << "   "                          \
            << "\e[0m" << STRINGIFY(x) << "\n";

#define GROUP(name)                                                            \
  std::cout << "\e[0m"                                                         \
            << "Test Group :: "                                                \
            << "\e[1;33m" << name << "\n";

int main() {
  using nuo::Maybe;
  using nuo::Vague;
  using nuo::Vec;

  GROUP("Vector")
  auto vec1 = Vec<int>({50, 60});
  auto vec2 = Vec<int>({100, 300});
  ASSERT(vec1[0] == 50) ASSERT(vec1[1] == 60) vec1.push(324);
  ASSERT(vec1[2] == 324) vec1.push(5434);
  ASSERT(vec1[3] == 5434)
  ASSERT(vec1.length() == 4)
  vec1.pushAll(vec2);
  ASSERT(vec1.length() == 6)
  vec1.pushAll({340, 930});
  ASSERT(vec1.length() == 8)
  vec1 = vec1 + vec2;
  ASSERT(vec1.length() == 10)

  GROUP("Maybe")
  auto opt1 = Maybe<int>();
  ASSERT(opt1.has() == false)
  ASSERT(opt1.getOr(10) == 10)
  opt1 = 5;
  ASSERT(opt1.has() == true)
  ASSERT(opt1.getOr(10) == 5)

  GROUP("Vague")
  auto vge1 = Vague<int>();
  vge1 = 32;
  ASSERT(vge1.getOr(50) == 32)

  /* Tests complete */
  return 0;
}
#include "nuo/exception.hpp"
#include "nuo/json.hpp"
#include "nuo/maybe.hpp"
#include "nuo/vague.hpp"
#include "nuo/vec.hpp"
#include <iostream>

#define STRINGIFY(a) str_val(a)

#define str_val(a) #a

#define ASSERT(x)                                                              \
  std::cout << "    " << ((x) ? "\e[0;32m" : "\e[1;31m")                       \
            << ((x) ? "success" : "failure") << "   "                          \
            << "\e[0m" << STRINGIFY(x) << "\n";

#define GROUP(name)                                                            \
  group = name;                                                                \
  std::cout << "\n\e[0m\e[1;43m Group :: " << name << " \e[0m"                 \
            << "\n";

#define SUBGROUP(name)                                                         \
  std::cout << "\e[1;33m  " << group << "\e[0m -> "                            \
            << "\e[1;34m" << name << "\e[0m"                                   \
            << "\n";

int main() {
  using nuo::Json;
  using nuo::Maybe;
  using nuo::Vague;
  using nuo::Vec;

  std::string group;

  GROUP("Vec")
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
  vec1 = (vec1 + vec2);
  ASSERT(vec1.length() == 10)
  SUBGROUP("Moving")
  auto getTempVec = []() { return Vec<int>({234, 324}); };
  vec1 = getTempVec();
  ASSERT(vec1[0] == 234)
  ASSERT(vec1[1] == 324)
  SUBGROUP("Copying")
  vec1 = vec2;
  ASSERT(vec1[0] == 100)
  ASSERT(vec1[1] == 300)
  SUBGROUP("Class Element")
  auto vec3 = Vec<Json>();
  vec3.push(R"({"hello" : "world"})"_json);
  ASSERT(vec3.size() == 1)
  ASSERT(vec3[0] == Json()._("hello", "world"))

  GROUP("Maybe")
  auto opt1 = Maybe<int>();
  ASSERT(opt1.has() == false)
  ASSERT(opt1.getOr(10) == 10)
  opt1 = 5;
  ASSERT(opt1.has() == true)
  ASSERT(opt1.getOr(10) == 5)
  auto opt2 = Maybe<int>();
  SUBGROUP("Moving")
  auto getTempMaybe = []() { return Maybe<int>(5676); };
  opt1 = getTempMaybe();
  ASSERT(opt1.has())
  ASSERT(opt1.get() == 5676)
  SUBGROUP("Copying")
  opt1 = opt2;
  ASSERT(opt1.has() == false)

  GROUP("Vague")
  auto vge1 = Vague<int>(34);
  ASSERT(vge1.has() == true)
  ASSERT(vge1.getOr(50) == 34)

  GROUP("Json")
  auto json = Json();
  json["hello"] = "hi";
  json["some"] = "someother";
  ASSERT(json["hello"] == "hi")
  ASSERT(json["hello"] != "some")
  json._("second", "other")._("third", "dru");
  ASSERT(json["second"] == "other")
  ASSERT(json["third"] == "dru")
  try { // Since we are using Json parsing, there can be exceptions
    SUBGROUP("Parsing & Literal Operator")
    auto jsn = R"({
            "hello": ["435345\"", "34435", "234234", {}],
            "hello3": {},
            "hello2":{"some":"dfg"}
          })"_json;

    ASSERT(jsn["hello"] ==
           std::vector<nuo::JsonValue>({"435345\"", "34435", "234234", Json()}))
    ASSERT(jsn["hello3"] == Json())
    ASSERT(jsn["hello2"] == Json()._("some", "dfg"))
    auto another = R"({"dfd": "some"})"_json;
    ASSERT(another["dfd"] == "some")
    SUBGROUP("Copying")
    jsn = another;
    ASSERT(jsn.size() == 1)
    ASSERT(jsn["dfd"] == "some")
  } catch (nuo::Exception &ex) {
    std::cout << ex.what() << "\n";
  }

  /* Tests complete */
  return 0;
}
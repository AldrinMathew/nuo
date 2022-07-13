#ifndef NUO_VAGUE_H
#define NUO_VAGUE_H

#include "nuo/exception.hpp"
#include "nuo/maybe.hpp"
#include <string>

namespace nuo {

// A Problem occured during logic
class Problem {
private:
  // Value of the problem
  std::string value;

public:
  // Create a Problem value
  Problem(std::string _val) : value(_val) {}

  // Copy constructor
  Problem(const Problem &other) : value(other.value) {}

  // Move constructor
  Problem(Problem &&other) : value(other.value) {}

  // Copy assignment
  void operator=(const Problem &other) { value = other.value; }

  // Move assignment
  void operator=(Problem &&other) { value = other.value; }

  // Get the problem described by this instance
  std::string get() { return value; }
};

// Vague is used when there is a possibility for a useful value, but also for a
// problem
template <typename T> class Vague {
private:
  // An optional value
  Maybe<T> value;

  // An optional problem
  Maybe<Problem> problem;

public:
  Vague() : value(), problem() {}

  /**
   * @brief Create a Vague instance with a valid value of the associated type
   *
   * @param value Value of the associated type
   */
  Vague(T _value) : value(_value), problem() {}

  // Create a Vague instance with a Problem
  Vague(Problem _problem) : value(), problem(_problem) {}

  // Copy constructor for Vague
  Vague(const Vague &other) : value(other.value), problem(other.problem) {}

  // Move constructor for Vague
  Vague(Vague &&other) : value(other.value), problem(other.problem) {
    other.value = Null();
    other.problem = Null();
  }

  ~Vague() noexcept {
    if (has()) {
      value.get().~T();
      value = Null();
    }
    if (hasProblem()) {
      problem.get().~Problem();
      problem = Null();
    }
  }

  // Assign reference to another const Vague instance
  void operator=(const Vague<T> &other) {
    value = other.value;
    problem = other.problem;
  }

  // Assign another value of the associated type
  void operator=(const T other_val) { value = other_val; }

  // Assign another problem
  void operator=(const Problem other_val) { problem = other_val; }

  // Whether there is proper value of the associated type stored in this
  // instance
  bool has() const noexcept { return value.has(); }

  // @brief Whether there is a problem in this instance
  bool hasProblem() const noexcept { return problem.has(); }

  // Get the value stored in this instance having the associated type.
  // This function throws NullAccessException when there is no value
  T solve(T (*fn)(Maybe<T>, Maybe<Problem>)) const noexcept {
    if (!hasProblem() && has()) {
      return value.get();
    } else {
      return fn(value, problem);
    }
  }

  // Ignore any problem in this instance and get the value directly
  Maybe<T> ignore() const noexcept { return value; }

  // If there is a proper value in this instance of the associated type,
  // that will be returned. Otherwise, the provided value is used
  T getOr(T other) const noexcept { return value.getOr(other); }

  // Get the optional Problem if there is any
  Maybe<Problem> getProblem() const noexcept { return problem; }
};

} // namespace nuo

#endif
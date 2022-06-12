#ifndef NUO_VAGUE_H
#define NUO_VAGUE_H

#include "nuo/exception.hpp"
#include "nuo/maybe.hpp"
#include <string>

namespace nuo {

/**
 * @brief A Problem occured during logic
 *
 */
class Problem {
private:
  /**
   * @brief Value of the problem
   *
   */
  std::string value;

public:
  /**
   * @brief Create a Problem value
   *
   * @param _val Value of the problem
   */
  Problem(std::string _val) : value(_val) {}

  /**
   * @brief Get the problem described by this instance
   *
   * @return std::string Value of the problem
   */
  std::string get() { return value; }
};

template <typename T> class Vague {
private:
  /**
   * @brief An optional value
   *
   */
  Maybe<T> value;

  /**
   * @brief An optional problem
   *
   */
  Maybe<Problem> problem;

public:
  Vague() noexcept : value(Null()), problem(Null()) {}

  /**
   * @brief Create a Vague instance with a valid value of the associated type
   *
   * @param value Value of the associated type
   */
  Vague(T _value) noexcept : value(_value), problem(Null()) {}

  /**
   * @brief Create a Vague instance with a Problem
   *
   * @param value An instance of Problem
   */
  Vague(Problem _problem) noexcept : value(Null()), problem(_problem) {}

  /**
   * @brief Copy constructor for Vague
   *
   * @param other Reference to the other Vague instance (lvalue)
   */
  Vague(const Vague &other) noexcept
      : value(other.value), problem(other.problem) {}

  /**
   * @brief Move constructor for Vague
   *
   * @param other Reference to the other Vague instance (rvalue)
   */
  Vague(Vague &&other) noexcept : value(other.value), problem(other.problem) {
    other.value = Null();
    other.problem = Null();
  }

  /**
   * @brief Destructor for Vague
   *
   */
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

  /**
   * @brief Assign reference to another const Vague instance
   *
   * @param other
   */
  void operator=(const Vague<T> &other) noexcept {
    value = other.value;
    problem = other.problem;
  }

  /**
   * @brief Assign another value of the associated type
   *
   * @param other_val Value of the associated type
   */
  void operator=(const T other_val) noexcept { value = other_val; }

  /**
   * @brief Assign another problem
   *
   * @param other_val A Problem value
   */
  void operator=(const Problem other_val) noexcept { problem = other_val; }

  /**
   * @brief Whether there is proper value of the associated type stored in this
   * instance
   *
   * @return true If there is no problem
   * @return false If there is a problem
   */
  bool has() const noexcept { return value.has(); }

  /**
   * @brief Whether there is a problem in this instance
   *
   * @return true If there is a problem
   * @return false If there is no problem
   */
  bool hasProblem() const noexcept { return problem.has(); }

  /**
   * @brief Get the value stored in this instance having the associated type.
   * This function throws NullAccessException when there is no value
   *
   * @return T The value with the associated type in this instance
   */
  T solve(T (*fn)(Maybe<T>, Maybe<Problem>)) const noexcept {
    if (!hasProblem() && has()) {
      return value.get();
    } else {
      return fn(value, problem);
    }
  }

  /**
   * @brief Ignore any problem in this instance and get the value directly
   *
   * @return Maybe<T> The value present
   */
  Maybe<T> ignore() const noexcept { return value; }

  /**
   * @brief If there is a proper value in this instance of the associated type,
   * that will be returned. Otherwise, the provided value is used
   *
   * @param other
   * @return T
   */
  T getOr(T other) const noexcept { return value.getOr(other); }

  /**
   * @brief Get the optional Problem if there is any
   *
   * @return Maybe<Problem> The optional problem
   */
  Maybe<Problem> getProblem() const noexcept { return problem; }
};

} // namespace nuo

#endif
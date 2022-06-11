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
  /**
   * @brief Create a Vague instance with a valid value of the associated type
   *
   * @param value Value of the associated type
   */
  Vague(T value) noexcept { this->value = value; }

  /**
   * @brief Create a Vague instance with a Problem
   *
   * @param value An instance of Problem
   */
  Vague(Problem problem) noexcept { this->problem = problem; }

  /**
   * @brief Copy constructor for Vague
   *
   * @param other Reference to the other Vague instance (lvalue)
   */
  Vague(Vague &other) noexcept {
    this->value = other.value;
    this->problem = other.problem;
  }

  /**
   * @brief Move constructor for Vague
   *
   * @param other Reference to the other Vague instance (rvalue)
   */
  Vague(Vague &&other) noexcept {
    this->value = other.value;
    this->problem = other.problem;
  }

  /**
   * @brief Destructor for Vague
   *
   */
  ~Vague() noexcept {
    if (has()) {
      val.t_val.~T();
    } else {
      val.p_val.~Problem();
    }
    has_val = false;
  }

  /**
   * @brief Assign reference to another const Vague instance
   *
   * @param other
   */
  void operator=(const Vague<T> &other) noexcept {
    val = other.val;
    has_val = other.has_val;
  }

  /**
   * @brief Assign another value of the associated type
   *
   * @param other_val Value of the associated type
   */
  void operator=(const T other_val) noexcept {
    val = other_val;
    has_val = true;
  }

  /**
   * @brief Assign another problem
   *
   * @param other_val A Problem value
   */
  void operator=(const Problem other_val) noexcept {
    val.p_val = other_val;
    has_val = false;
  }

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
    return fn(value, problem);
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
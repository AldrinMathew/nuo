#ifndef NUO_MAYBE_HPP
#define NUO_MAYBE_HPP

#include "nuo/exception.hpp"

namespace nuo {

/**
 * @brief A type representing null value
 *
 */
class Null {};

/**
 * @brief An Maybe<T> instance can be used to represent a value that might not
 * have a valid value of type T. This emulates the behaviour of null references
 * in high-level languages, but without leading the problems that causes. You
 * have to manually handle and check if there is a valid value
 *
 * @tparam T Type to associate this Maybe instance to
 */
template <typename T> class Maybe {
private:
  /**
   * @brief maybe_base is a shadow type to maintain the value. The user does
   * not need to access this type or its instance directly.
   *
   */
  union maybe_base {
    T t_val;
    Null n_val;
  } val;

  /**
   * @brief Whether there is a value of the associated type
   *
   */
  bool has_val;

public:
  /**
   * @brief Create an Maybe instance with a value of the associated type
   *
   * @param value Value of type T
   */
  Maybe(T value) noexcept {
    has_val = true;
    val.t_val = value;
  }

  /**
   * @brief Create an Maybe instance with a null value
   *
   * @param value An instance of Null. Defaults to null
   */
  Maybe(Null value = Null()) noexcept {
    has_val = false;
    val.n_val = value;
  }

  /**
   * @brief Copy constructor for Maybe
   *
   * @param other Reference to the other Maybe instance (lvalue)
   */
  Maybe(Maybe<T> &other) noexcept {
    val = other.val;
    has_val = other.has_val;
  }

  /**
   * @brief Move constructor for Maybe
   *
   * @param other Reference to the other Maybe instance (rvalue)
   */
  Maybe(Maybe<T> &&other) noexcept {
    val = other.val;
    has_val = other.has_val;
  }

  /**
   * @brief Destuctor for Maybe
   *
   */
  ~Maybe() noexcept {
    if (has()) {
      val.t_val.~T();
    } else {
      val.n_val.~Null();
    }
    has_val = false;
  }

  /**
   * @brief Copy assignment operator
   *
   * @param other
   */
  void operator=(const Maybe<T> &other) noexcept {
    val = other.val;
    has_val = other.has_val;
  }

  /**
   * @brief Move assignment operator
   *
   * @param other The temporary value (rvalue)
   */
  void operator=(Maybe<T> &&other) noexcept {
    val = other.val;
    has_val = other.has_val;
  }

  /**
   * @brief Assign a value of the associated type
   *
   * @param other_val
   */
  void operator=(const T other_val) noexcept {
    val.t_val = other_val;
    has_val = true;
  }

  /**
   * @brief Assign a null value to this instance
   *
   * @param other_val
   */
  void operator=(const Null other_val) noexcept {
    val.n_val = other_val;
    has_val = false;
  }

  /**
   * @brief Whether there is value of the associated type stored in this
   * instance
   *
   * @return true If the value is not null
   * @return false If the value is null
   */
  bool has() const noexcept { return has_val; }

  /**
   * @brief Get the value stored in this instance having the associated type.
   * This function throws NullAccessException when there is no value
   *
   * @return T The value with the associated type in this instance
   */
  T get() const {
    if (has()) {
      return val.t_val;
    } else {
      throw Exception("Tried to access Maybe value when it is null");
    }
  }

  /**
   * @brief If there is a value in this instance of the associated type, that
   * will be returned. Otherwise, the provided value is used
   *
   * @param other
   * @return T
   */
  T getOr(T other) const noexcept { return has() ? val.t_val : other; }
};

} // namespace nuo
#endif
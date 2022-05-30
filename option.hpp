#ifndef OPTION_HPP
#define OPTION_HPP

#include <exception>

/**
 * @brief A type representing null value
 *
 */
class NullTy {};

/**
 * @brief A value of NullTy type
 *
 */
const auto Null = NullTy();

/**
 * @brief
 *
 */
class NullAccessException : public std::exception {
public:
  const char *what() { return "Tried to access value when it is null"; }
};

template <typename T> class Option {
private:
  /**
   * @brief optional_base is a shadow type to maintain the value. The user does
   * not need to access this type or its instance directly.
   *
   */
  union optional_base {
    T t_val;
    NullTy n_val;
  } val;

  /**
   * @brief Whether there is a value of the associated type
   *
   */
  bool has_val;

public:
  Option(T value) noexcept {
    has_val = true;
    val.t_val = value;
  }

  /**
   * @brief Create an Option instance with a null value
   *
   * @param value An instance of NullTy. Defaults to null
   */
  Option(NullTy value = Null) noexcept {
    has_val = false;
    val.n_val = value;
  }

  /**
   * @brief Copy constructor for Option
   *
   * @param other Reference to the other Option instance (lvalue)
   */
  Option(Option &other) noexcept {
    val = other.val;
    has_val = other.has_val;
  }

  /**
   * @brief Move constructor for Option
   *
   * @param other Reference to the other Option instance (rvalue)
   */
  Option(Option &&other) noexcept {
    val = other.val;
    has_val = other.has_val;
  }

  /**
   * @brief Assign reference to another const Option instance
   *
   * @param other
   */
  void operator=(const Option<T> &other) noexcept {
    val = other.val;
    has_val = other.has_val;
  }

  void operator=(const T other_val) noexcept {
    val.t_val = other_val;
    has_val = true;
  }

  void operator=(const NullTy other_val) noexcept {
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
      throw NullAccessException();
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

#endif
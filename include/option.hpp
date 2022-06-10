#ifndef NUO_OPTION_HPP
#define NUO_OPTION_HPP

#include <exception>
namespace nuo {

/**
 * @brief A type representing null value
 *
 */
class Null {};

/**
 * @brief
 *
 */
class NullAccessException : public std::exception {
public:
  const char *what();
};

/**
 * @brief An Option<T> instance can be used to represent a value that might not
 * have a valid value of type T. This emulates the behaviour of null references
 * in high-level languages, but without leading the problems that causes. You
 * have to manually handle and check if there is a valid value
 *
 * @tparam T Type to associate this Option instance to
 */
template <typename T> class Option {
private:
  /**
   * @brief optional_base is a shadow type to maintain the value. The user does
   * not need to access this type or its instance directly.
   *
   */
  union optional_base {
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
   * @brief Create an Option instance with a value of the associated type
   *
   * @param value Value of type T
   */
  Option(T value) noexcept;

  /**
   * @brief Create an Option instance with a null value
   *
   * @param value An instance of Null. Defaults to null
   */
  Option(Null value = Null()) noexcept;

  /**
   * @brief Copy constructor for Option
   *
   * @param other Reference to the other Option instance (lvalue)
   */
  Option(Option<T> &other) noexcept;

  /**
   * @brief Move constructor for Option
   *
   * @param other Reference to the other Option instance (rvalue)
   */
  Option(Option<T> &&other) noexcept;

  /**
   * @brief Destuctor for Option
   *
   */
  ~Option() noexcept;

  /**
   * @brief Assign reference to another const Option instance
   *
   * @param other
   */
  void operator=(const Option<T> &other) noexcept;

  /**
   * @brief Assign a value of the associated type
   *
   * @param other_val
   */
  void operator=(const T other_val) noexcept;

  /**
   * @brief Assign a null value to this instance
   *
   * @param other_val
   */
  void operator=(const Null other_val) noexcept;

  /**
   * @brief Whether there is value of the associated type stored in this
   * instance
   *
   * @return true If the value is not null
   * @return false If the value is null
   */
  bool has() const noexcept;

  /**
   * @brief Get the value stored in this instance having the associated type.
   * This function throws NullAccessException when there is no value
   *
   * @return T The value with the associated type in this instance
   */
  T get() const;

  /**
   * @brief If there is a value in this instance of the associated type, that
   * will be returned. Otherwise, the provided value is used
   *
   * @param other
   * @return T
   */
  T getOr(T other) const noexcept;
};

} // namespace nuo
#endif
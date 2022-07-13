#ifndef NUO_MAYBE_HPP
#define NUO_MAYBE_HPP

#include "nuo/exception.hpp"

namespace nuo {

// A type representing null value
class Null {};

// An Maybe<T> instance can be used to represent a value that might not
// have a valid value of type T. This emulates the behaviour of null references
// in high-level languages, but without leading the problems that causes. You
// have to manually handle and check if there is a valid value
template <typename T> class Maybe {
private:
  void *val;

public:
  // Create a Maybe instance with a value of the associated type
  Maybe(const T value) : val(new T(value)) {}

  // Create a Maybe instance with a null value
  Maybe() : val(nullptr) {}

  // Copy constructor for Maybe
  Maybe(const Maybe<T> &other) {
    if (val) {
      if (other.has()) {
        *((T *)val) = *((T *)other.val);
      } else {
        delete ((T *)val);
        val = nullptr;
      }
    } else {
      val = new T(*((T *)other.val));
    }
  }

  // Move constructor for Maybe
  Maybe(Maybe<T> &&other) : val(other.val) { other.val = nullptr; }

  ~Maybe() {
    if (val) {
      delete ((T *)val);
      val = nullptr;
    }
  }

  // brief Copy assignment operator
  void operator=(const Maybe<T> &other) {
    if (val) {
      if (other.has()) {
        *((T *)val) = *((T *)other.val);
      } else {
        delete ((T *)val);
        val = nullptr;
      }
    } else {
      val = new T(*((T *)other.val));
    }
  }

  // Move assignment operator
  void operator=(Maybe<T> &&other) {
    if (val) {
      delete ((T *)val);
    }
    val = other.val;
    other.val = nullptr;
  }

  // Assign a value of the associated type
  void operator=(const T other_val) {
    if (val) {
      *((T *)val) = other_val;
    } else {
      val = new T(other_val);
    }
  }

  // Assign a null value to this instance
  void operator=(const Null other_val) {
    if (val) {
      delete ((T *)val);
    }
    val = nullptr;
  }

  // Whether there is value of the associated type stored in this instance
  bool has() const { return (val != nullptr); }

  /// Get the value stored in this instance having the associated type. This
  /// function throws NullAccessException when there is no value
  ///
  /// T The value with the associated type in this instance
  T get() const {
    if (has()) {
      return *((T *)val);
    } else {
      throw Exception("Tried to access Maybe value when it is null");
    }
  }

  /// If there is a value in this instance of the associated type, that will be
  /// returned. Otherwise, the provided value is used
  T getOr(T other) const { return has() ? *((T *)val) : other; }
};

} // namespace nuo
#endif
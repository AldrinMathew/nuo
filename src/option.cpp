#include "option.hpp"

namespace nuo {

const char *NullAccessException::what() {
  return "Tried to access Option value when it is null";
}

template <typename T> Option<T>::Option(T value) noexcept {
  has_val = true;
  val.t_val = value;
}

template <typename T> Option<T>::Option(Null value) noexcept {
  has_val = false;
  val.n_val = value;
}

template <typename T> Option<T>::Option(Option<T> &other) noexcept {
  val = other.val;
  has_val = other.has_val;
}

template <typename T> Option<T>::Option(Option<T> &&other) noexcept {
  val = other.val;
  has_val = other.has_val;
}

template <typename T> Option<T>::~Option() noexcept {
  val.n_val = Null();
  has_val = false;
}

template <typename T>
void Option<T>::operator=(const Option<T> &other) noexcept {
  val = other.val;
  has_val = other.has_val;
}

template <typename T> void Option<T>::operator=(const T other_val) noexcept {
  val.t_val = other_val;
  has_val = true;
}

template <typename T> void Option<T>::operator=(Null other_val) noexcept {
  val.n_val = other_val;
  has_val = false;
}

template <typename T> bool Option<T>::has() const noexcept { return has_val; }

template <typename T> T Option<T>::get() const {
  if (has()) {
    return val.t_val;
  } else {
    throw NullAccessException();
  }
}

template <typename T> T Option<T>::getOr(T other) const noexcept {
  return has() ? val.t_val : other;
}

} // namespace nuo
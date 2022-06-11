#ifndef NUO_VEC_HPP
#define NUO_VEC_HPP

#include "nuo/maybe.hpp"
#include <initializer_list>

namespace nuo {

/**
 * @brief A heap allocated vector containing elements of the associated type
 *
 * @tparam T The element type of the vector
 */
template <typename T> class Vec {
private:
  /**
   * @brief The starting pointer
   *
   */
  T *start;

  /**
   * @brief The number of elements in the vector
   *
   */
  unsigned len;

  /**
   * @brief Length of the buffer, or the number of elements that could be stored
   * in the vector without resizing
   *
   */
  unsigned buff_len;

public:
  /**
   * @brief Create an empty vector
   *
   */
  Vec() : start(nullptr), len(0), buff_len(0) {}

  /**
   * @brief Create a vector with space allocated for the provided number of
   * elements
   *
   * @param count Number of elements to allocate space for
   */
  Vec(unsigned count)
      : start((T *)malloc(count * sizeof(T))), len(0), buff_len(count) {}

  /**
   * @brief Copy constructor for Vec<T>
   *
   * @param other The reference to the other Vec instance
   */
  Vec(const Vec &other)
      : start(other.start), len(other.len), buff_len(other.buff_len) {}

  /**
   * @brief Move constructor for Vec<T>
   *
   * @param other The temporary value
   */
  Vec(const Vec &&other)
      : start(other.start), len(other.len), buff_len(other.buff_len) {}

  /**
   * @brief Create a vector containing the elements in the provided initialiser
   * list
   *
   * @param list
   */
  Vec(std::initializer_list<T> list) : start(nullptr), len(0), buff_len(0) {
    start = (T *)malloc(list.size() * sizeof(T));
    len = list.size();
    buff_len = len;
    unsigned i = 0;
    for (auto element = list.begin(); element != list.end(); element++) {
      start[i] = *element;
      i++;
    }
  }

  /**
   * @brief Push a new element after the last element. This will resize the
   * buffer of the vector if there is no preallocated space available
   *
   * @param element
   */
  void push(T element) noexcept {
    if ((!start) || (buff_len == 0)) {
      start = (T *)malloc(2 * sizeof(T));
      len = 0;
      buff_len = 2;
    }
    // If there is no more space, move the elements to a new memory
    // location with space for more elements
    if ((len == buff_len) && start) {
      buff_len = 2 * buff_len;
      auto new_start = (T *)malloc(sizeof(T) * buff_len);
      for (unsigned i = 0; i < len; i++) {
        new_start[i] = start[i];
      }
      free(start);
      start = new_start;
    }
    start[len] = element;
    len++;
  }

  /**
   * @brief Pop the last element of the vector. This calls the destructor of the
   * last element.
   *
   */
  void pop() noexcept {
    if (len > 0) {
      start[len].~T();
      --len;
    }
  }

  /**
   * @brief The number of elements in the vector
   *
   * @return unsigned
   */
  unsigned length() const noexcept { return len; }

  /**
   * @brief Get the element at the provided index. This does not throw errors if
   * the index is out of range. Instead the returned value is of Maybe<T> type.
   * This also does not make any changes to the element.
   *
   * @param index Index to find the element at
   * @return Maybe<T>
   */
  Maybe<T> at(unsigned index) const noexcept {
    if (index < len) {
      return Maybe<T>(start[index]);
    } else {
      return Null();
    }
  }

  /**
   * @brief Get the element at the provided index. This will throw an error if
   * the provided index is out of range. But this is not a const function since
   * you can modify an element value
   *
   * @param index
   * @return T&
   */
  T &operator[](unsigned index) {
    if (index < len) {
      return start[index];
    } else {
      throw Exception("Index out of range while accessing element in Vec");
    }
  }

  /**
   * @brief Clear all elements of the vector
   *
   */
  void clear() noexcept {
    for (unsigned i = 0; i < len; i++) {
      start[i].~T();
    }
    len = 0;
    buff_len = 0;
    free(start);
    start = nullptr;
  }

  /**
   * @brief Execute the provided function for each and every element in this
   * vector
   *
   * @param fn The function that should be executed. The first argument is the
   * reference to a const value of type T
   */
  void forEach(void (*fn)(const T &)) {
    for (unsigned i = 0; i < len; i++) {
      fn(start[i]);
    }
  }

  /**
   * @brief Map each element of this vector to a new datatype and convert the
   * resultant sequence to a new vector
   *
   * @tparam K The type of element of the resultant vector
   * @param fn
   * @return Vec<K>
   */
  template <typename K> Vec<K> mapEach(K (*fn)(const T &)) {
    auto result = Vec<K>(len);
    for (unsigned i = 0; i < len; i++) {
      result.push(fn(start[i]));
    }
    return result;
  }

  /**
   * @brief Number of elements to allocate space for. If this is called when the
   * vector is not empty, all elements will be cleared.
   *
   * @param count Number of elements
   */
  void reallocate(unsigned count) {
    forEach([](const T &elem) { elem.~T(); });
    free(start);
    start = (T *)malloc(count * sizeof(T));
    len = 0;
    buff_len = count;
  }

  /**
   * @brief Execute the provided function for each and every element in this
   * vector
   *
   * @param fn The function that should be executed. This should have 2
   * arguments. The first one is the reference to a const value of type T, and
   * the second argument is the index of the element in the vector
   */
  void forEachIndex(void (*fn)(const T &, unsigned)) {
    for (unsigned i = 0; i < len; i++) {
      fn(start[i], i);
    }
  }

  /**
   * @brief Get the buffer length of the vector. This represents the number of
   * elements for which the space has already been allocated.
   *
   * @return unsigned
   */
  unsigned get_buffer_length() const noexcept { return buff_len; }

  /**
   * @brief Copy assignment
   *
   * @param other Other value
   *
   */
  void operator=(const Vec<T> &other) noexcept {
    for (unsigned i = 0; i < len; i++) {
      start[i].~T();
    }
    free(start);
    start = other.start;
    len = other.len;
    buff_len = other.buff_len;
  }

  /**
   * @brief Move assignment
   *
   * @param other Other value
   */
  void operator=(Vec<T> &&other) noexcept {
    for (unsigned i = 0; i < len; i++) {
      start[i].~T();
    }
    free(start);
    start = other.start;
    len = other.len;
    buff_len = other.buff_len;
  }

  ~Vec() noexcept {
    for (unsigned i = 0; i < len; i++) {
      start[i].~T();
    }
    len = 0;
    buff_len = 0;
    free(start);
    start = nullptr;
  }
};

} // namespace nuo

#endif
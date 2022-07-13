#ifndef NUO_VEC_HPP
#define NUO_VEC_HPP

#include "nuo/maybe.hpp"
#include <cstddef>
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

  T *allocate_space(unsigned count) { return (T *)malloc(count * sizeof(T)); }

  void free_space() {
    free(start);
    start = nullptr;
  }

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
  Vec(unsigned count) : start(allocate_space(count)), len(0), buff_len(count) {}

  /**
   * @brief Copy constructor for Vec<T>
   *
   * @param other The reference to the other Vec instance
   */
  Vec(const Vec<T> &other) : start(nullptr), len(0), buff_len(0) {
    start = allocate_space(other.len);
    for (unsigned i = 0; i < other.len; i++) {
      start[i] = other.at(i).get();
    }
    len = other.len;
    buff_len = len;
  }

  /**
   * @brief Move constructor for Vec<T>
   *
   * @param other The temporary value
   */
  Vec(Vec<T> &&other)
      : start(other.start), len(other.len), buff_len(other.buff_len) {
    other.start = nullptr;
    other.len = 0;
    other.buff_len = 0;
  }

  /**
   * @brief Create a vector containing the elements in the provided initialiser
   * list
   *
   * @param list
   */
  Vec(std::initializer_list<T> list) : start(nullptr), len(0), buff_len(0) {
    start = allocate_space(list.size());
    len = list.size();
    buff_len = len;
    unsigned i = 0;
    for (const auto &elem : list) {
      start[i] = elem;
      i++;
    }
  }

  class iterator {
  private:
    std::size_t position;
    const Vec<T> *parent;

  public:
    iterator(const Vec<T> *_parent, std::size_t _position)
        : position(_position), parent(_parent) {}

    iterator(const iterator &other)
        : position(other.position), parent(other.parent) {}

    iterator(iterator &&other)
        : position(other.position), parent(other.parent) {}

    void operator=(const iterator &other) {
      position = other.position;
      parent = other.parent;
    }

    void operator=(iterator &&other) {
      position = other.position;
      parent = other.parent;
      other.position = 0;
      other.parent = nullptr;
    }

    ~iterator() noexcept {
      position = 0;
      parent = nullptr;
    }

    iterator &operator--() noexcept {
      if (position > 0) {
        position--;
      }
      return *this;
    }

    iterator &operator--(int) noexcept {
      iterator tmp = *this;
      if (position > 0) {
        position--;
      }
      return tmp;
    }

    iterator &operator++() noexcept {
      if (parent) {
        if (position < parent->len) {
          position++;
        }
      }
      return *this;
    }

    iterator &operator++(int) noexcept {
      iterator tmp = *this;
      if (parent) {
        if (position < parent->len) {
          position++;
        }
      }
      return tmp;
    }

    bool operator==(iterator other) const {
      return ((position == other.position) && (parent == other.parent));
    }

    bool operator!=(iterator other) const {
      return (position != other.position);
    }

    T operator*() const noexcept { return parent->at(position).get(); }
  };

  iterator begin() const { return iterator(this, 0); }

  iterator end() const { return iterator(this, len); }

  /**
   * @brief Push a new element after the last element. This will resize the
   * buffer of the vector if there is no preallocated space available
   *
   * @param element
   */
  void push(T element) noexcept {
    if ((!start) || (buff_len == 0)) {
      start = allocate_space(2);
      len = 0;
      buff_len = 2;
    }
    // If there is no more space, move the elements to a new memory
    // location with space for more elements
    if ((len == buff_len) && start) {
      buff_len = 2 * ((buff_len > 0) ? buff_len : 1);
      auto new_start = allocate_space(buff_len);
      for (unsigned i = 0; i < len; i++) {
        new_start[i] = start[i];
      }
      free_space();
      start = new_start;
    }
    start[len] = element;
    len++;
  }

  void pushAll(std::initializer_list<T> elements) noexcept {
    for (const auto &elem : elements) {
      push(elem);
    }
  }

  void pushAll(const Vec<T> &other) noexcept {
    for (unsigned i = 0; i < other.len; i++) {
      push(other.at(i).get());
    }
  }

  void pushAll(Vec<T> &&other) noexcept {
    for (unsigned i = 0; i < other.len; i++) {
      push(std::move(other[i]));
    }
    other.clear();
    other.start = nullptr;
    other.len = 0;
    other.buff_len = 0;
  }

  Vec<T> operator+(const Vec<T> &other) const noexcept {
    auto result = Vec<T>(*this);
    if (other.size() != 0) {
      result.pushAll(other);
    }
    return result;
  }

  Vec<T> operator+(const std::initializer_list<T> &elements) const noexcept {
    auto result = Vec<T>(*this);
    if (elements.size() != 0) {
      for (const auto &elem : elements) {
        result.push(elem);
      }
    }
    return result;
  }

  /**
   * @brief Pop the last element of the vector. This calls the destructor of
   * the last element.
   *
   */
  void pop() noexcept(true) {
    if (len > 0) {
      start[len].~T();
      --len;
    }
  }

  // The number of elements in the vector
  unsigned length() const noexcept(true) { return len; }

  unsigned size() const noexcept(true) { return len; }

  /**
   * @brief Get the element at the provided index. This does not throw errors if
   * the index is out of range. Instead the returned value is of Maybe<T> type.
   * This also does not make any changes to the element.
   *
   * @param index Index to find the element at
   * @return Maybe<T>
   */
  Maybe<T> at(unsigned index) const noexcept(true) {
    if (index < len) {
      return Maybe<T>(start[index]);
    } else {
      return Maybe<T>();
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
    if (start) {
      for (unsigned i = 0; i < len; i++) {
        start[i].~T();
      }
      free_space();
    }
    len = 0;
    buff_len = 0;
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
    if (start) {
      forEach([](const T &elem) { elem.~T(); });
      free_space();
      start = nullptr;
    }
    start = allocate_space(count);
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
    free_space();
    start = allocate_space(other.len);
    for (unsigned i = 0; i < len; i++) {
      start[i] = other.at(i).get();
    }
    len = other.len;
    buff_len = len;
  }

  /**
   * @brief Move assignment
   *
   * @param other Other value
   */
  void operator=(Vec<T> &&other) noexcept {
    start = other.start;
    len = other.len;
    buff_len = other.buff_len;
    other.start = nullptr;
    other.len = 0;
    other.buff_len = 0;
  }

  ~Vec() noexcept {
    if (start) {
      for (unsigned i = 0; i < len; i++) {
        start[i].~T();
      }
      free_space();
    }
    len = 0;
    buff_len = 0;
    start = nullptr;
  }
};

} // namespace nuo

#endif
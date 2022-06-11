#ifndef NUO_EXCEPTION_HPP
#define NUO_EXCEPTION_HPP

#include <exception>
#include <string>

namespace nuo {

class Exception : std::exception {
private:
  /**
   * @brief Message in the exception
   *
   */
  std::string message;

public:
  /**
   * @brief Create an Exception with the provided message
   *
   * @param message Message of the exception
   */
  Exception(std::string message);

  /**
   * @brief This can be used to get the message by users
   *
   * @return const char* Message in C string format
   */
  const char *what() const noexcept;
};

} // namespace nuo

#endif
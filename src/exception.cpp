#include "nuo/exception.hpp"

namespace nuo {

Exception::Exception(std::string _message) : message(_message) {}

const char *Exception::what() const noexcept { return message.c_str(); }

} // namespace nuo
#ifndef NUO_JSON_HPP
#define NUO_JSON_HPP

#include <cstdint>
#include <initializer_list>
#include <string>
#include <vector>

namespace nuo {

enum class JsonValueType {
  integer,
  decimal,
  string,
  boolean,
  null,
  json,
  none,
  list
};

class Json;

class JsonValue {
protected:
  friend class Json;

  void *data;

  JsonValueType type;

  JsonValue(JsonValueType type, void *data);

public:
  JsonValue();

  void operator=(const std::nullptr_t ptr);

  JsonValue(int val);

  void operator=(const int val);

  JsonValue(unsigned val);

  void operator=(const unsigned val);

  JsonValue(uint64_t val);

  void operator=(const uint64_t val);

  JsonValue(int64_t val);

  void operator=(const int64_t val);

  JsonValue(double val);

  void operator=(const double val);

  JsonValue(std::string val);

  void operator=(const std::string val);

  JsonValue(const char *val);

  void operator=(const char *val);

  JsonValue(bool val);

  void operator=(const bool val);

  JsonValue(Json val);

  void operator=(Json val);

  JsonValue(std::vector<JsonValue> val);

  void operator=(std::vector<JsonValue> val);

  JsonValue(std::initializer_list<JsonValue> val);

  void operator=(std::initializer_list<JsonValue> val);

  JsonValue(JsonValue const &other);

  void operator=(JsonValue const &other);

  JsonValue(JsonValue &&other);

  void operator=(JsonValue &&other);

  static JsonValue none();

  explicit operator bool() const;

  bool operator==(const JsonValue &other) const;

  bool operator!=(const JsonValue &other) const;

  std::string toString() const;

  JsonValueType getType() const;

  template <typename T> T cast() { return *((T *)data); }

  bool isInt() const;

  bool isDouble() const;

  bool isNull() const;

  bool isString() const;

  bool isBool() const;

  bool isJson() const;

  bool isNone() const;

  bool isList() const;

  friend std::ostream &operator<<(std::ostream &os, const JsonValue &val);

  void clear();

  ~JsonValue() noexcept;
};

class Json {
private:
  std::vector<std::string> keys;
  std::vector<JsonValue> values;

  mutable unsigned level = 0;
  mutable unsigned spaces = 2;

  void setLevel(unsigned lev) const;

public:
  Json();

  Json(Json const &other);

  Json(Json &&other);

  Json &_(const std::string key, const JsonValue val);

  void operator=(Json const &other);

  void operator=(Json &&other);

  void setSpaces(unsigned spc) const;

  std::string toString() const;

  bool has(const std::string key) const;

  JsonValue &operator[](const std::string key);

  bool operator==(const Json &other) const;

  bool operator!=(const Json &other) const;

  std::size_t size() const;

  friend std::ostream &operator<<(std::ostream &os, const Json &dt);

  void clear() noexcept;

  ~Json() noexcept;
};

} // namespace nuo

#endif
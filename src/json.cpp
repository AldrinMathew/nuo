#include "nuo/json.hpp"
#include "nuo/json_parser.hpp"
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <vector>

namespace nuo {

JsonValue::JsonValue(JsonValueType _type, void *_data)
    : data(_data), type(_type) {}

JsonValue JsonValue::none() { return JsonValue(JsonValueType::none, nullptr); }

JsonValue::operator bool() const { return (type != JsonValueType::none); }

JsonValue::JsonValue() : data(nullptr), type(JsonValueType::null) {}

JsonValue::JsonValue(const int val)
    : data(new int64_t(val)), type(JsonValueType::integer) {}

void JsonValue::operator=(const int val) {
  clear();
  type = JsonValueType::integer;
  data = new int64_t(val);
}

JsonValue::JsonValue(const unsigned val)
    : data(new int64_t((int64_t)val)), type(JsonValueType::integer) {}

void JsonValue::operator=(const unsigned val) {
  clear();
  type = JsonValueType::integer;
  data = new int64_t((int64_t)val);
}

JsonValue::JsonValue(const unsigned long long val)
    : data(new int64_t((int64_t)val)), type(JsonValueType::integer) {}

void JsonValue::operator=(const unsigned long long val) {
  clear();
  type = JsonValueType::integer;
  data = new int64_t((int64_t)val);
}

JsonValue::JsonValue(const uint64_t val)
    : data(new int64_t((int64_t)val)), type(JsonValueType::integer) {}

void JsonValue::operator=(const uint64_t val) {
  clear();
  type = JsonValueType::integer;
  data = new int64_t((int64_t)val);
}

JsonValue::JsonValue(const int64_t val)
    : data(new int64_t(val)), type(JsonValueType::integer) {}

void JsonValue::operator=(const int64_t val) {
  clear();
  type = JsonValueType::integer;
  data = new int64_t(val);
}

JsonValue::JsonValue(const double val)
    : data(new double(val)), type(JsonValueType::decimal) {}

void JsonValue::operator=(const double val) {
  clear();
  type = JsonValueType::decimal;
  data = new double(val);
}

JsonValue::JsonValue(const std::string val)
    : data(new std::string(val)), type(JsonValueType::string) {}

void JsonValue::operator=(const std::string val) {
  clear();
  type = JsonValueType::string;
  data = new std::string(val);
}

JsonValue::JsonValue(const char *val)
    : data(new std::string(val)), type(JsonValueType::string) {}

void JsonValue::operator=(const char *val) {
  clear();
  type = JsonValueType::string;
  data = new std::string(val);
}

JsonValue::JsonValue(const bool val)
    : data(new bool(val)), type(JsonValueType::boolean) {}

void JsonValue::operator=(const bool val) {
  clear();
  type = JsonValueType::boolean;
  data = new bool(val);
}

JsonValue::JsonValue(const Json val)
    : data(new Json(val)), type(JsonValueType::json) {}

void JsonValue::operator=(Json val) {
  clear();
  type = JsonValueType::json;
  data = new Json(val);
}

JsonValue::JsonValue(const std::vector<JsonValue> val)
    : data(new std::vector<JsonValue>()), type(JsonValueType::list) {
  for (auto elem : val) {
    ((std::vector<JsonValue> *)data)->push_back(elem);
  }
}

void JsonValue::operator=(const std::vector<JsonValue> val) {
  clear();
  type = JsonValueType::list;
  data = new std::vector<JsonValue>(val);
}

JsonValue::JsonValue(const std::initializer_list<JsonValue> val)
    : data(new std::vector<JsonValue>()), type(JsonValueType::list) {
  for (auto &elem : val) {
    ((std::vector<JsonValue> *)data)->push_back(elem);
  }
}

void JsonValue::operator=(const std::initializer_list<JsonValue> val) {
  clear();
  type = JsonValueType::list;
  data = new std::vector<JsonValue>();
  for (auto &elem : val) {
    ((std::vector<JsonValue> *)data)->push_back(elem);
  }
}

JsonValue::JsonValue(JsonValue &&other) {
  clear();
  type = other.type;
  data = other.data;
  other.data = nullptr;
  other.type = JsonValueType::none;
}

void JsonValue::operator=(JsonValue &&other) {
  clear();
  type = other.type;
  data = other.data;
  other.data = nullptr;
  other.type = JsonValueType::none;
}

JsonValue::JsonValue(JsonValue const &other)
    : data(nullptr), type(JsonValueType::none) {
  clear();
  type = other.type;
  switch (type) {
  case JsonValueType::integer: {
    data = new int64_t(*((int64_t *)other.data));
    break;
  }
  case JsonValueType::decimal: {
    data = new double(*((double *)other.data));
    break;
  }
  case JsonValueType::string: {
    data = new std::string(*((std::string *)other.data));
    break;
  }
  case JsonValueType::boolean: {
    data = new bool(*((bool *)other.data));
    break;
  }
  case JsonValueType::json: {
    data = new Json(*((Json *)other.data));
    break;
  }
  case JsonValueType::list: {
    auto vals = (std::vector<JsonValue> *)other.data;
    data = new std::vector<JsonValue>(*vals);
    break;
  }
  case JsonValueType::null:
  case JsonValueType::none: {
    break;
  }
  }
}

void JsonValue::operator=(JsonValue const &other) {
  clear();
  type = other.type;
  switch (type) {
  case JsonValueType::integer: {
    data = new int64_t(*((int64_t *)other.data));
    break;
  }
  case JsonValueType::decimal: {
    data = new double(*((double *)other.data));
    break;
  }
  case JsonValueType::string: {
    data = new std::string(*((std::string *)other.data));
    break;
  }
  case JsonValueType::boolean: {
    data = new bool(*((bool *)other.data));
    break;
  }
  case JsonValueType::json: {
    data = new Json(*((Json *)other.data));
    break;
  }
  case JsonValueType::list: {
    auto vals = (std::vector<JsonValue> *)other.data;
    data = new std::vector<JsonValue>();
    for (std::size_t i = 0; i < vals->size(); i++) {
      ((std::vector<JsonValue> *)data)->push_back(vals->at(i));
    }
    break;
  }
  case JsonValueType::null:
  case JsonValueType::none: {
    break;
  }
  }
}

bool JsonValue::operator==(const JsonValue &other) const {
  if (type != other.type) {
    return false;
  }
  switch (type) {
  case JsonValueType::integer: {
    return (*((int64_t *)data)) == (*((int64_t *)other.data));
  }
  case JsonValueType::decimal: {
    return (*((double *)data)) == (*((double *)other.data));
  }
  case JsonValueType::string: {
    return (*((std::string *)data)) == (*((std::string *)other.data));
  }
  case JsonValueType::boolean: {
    return (*((bool *)data)) == (*((bool *)other.data));
  }
  case JsonValueType::json: {
    return (*((Json *)data)) == (*((Json *)other.data));
  }
  case JsonValueType::list: {
    auto otherVals = (std::vector<JsonValue> *)other.data;
    auto vals = (std::vector<JsonValue> *)data;
    if (otherVals->size() != vals->size()) {
      return false;
    }
    for (std::size_t i = 0; i < vals->size(); i++) {
      if (vals->at(i) != otherVals->at(i)) {
        return false;
      }
    }
    return true;
  }
  case JsonValueType::null:
  case JsonValueType::none: {
    return true;
  }
  }
}

bool JsonValue::operator!=(const JsonValue &other) const {
  return !(*this == other);
}

std::string JsonValue::toString() const {
  switch (type) {
  case JsonValueType::string: {
    return '"' + (*((std::string *)data)) + '"';
  }
  case JsonValueType::integer: {
    return std::to_string(*((int64_t *)data));
  }
  case JsonValueType::decimal: {
    return std::to_string(*((double *)data));
  }
  case JsonValueType::boolean: {
    return (*((bool *)data)) ? "true" : "false";
  }
  case JsonValueType::json: {
    return ((Json *)data)->toString();
  }
  case JsonValueType::list: {
    std::string result("[");
    auto list = (std::vector<JsonValue> *)data;
    for (std::size_t i = 0; i < list->size(); i++) {
      result += list->at(i).toString();
      if (i != (list->size() - 1)) {
        result += ", ";
      }
    }
    result += "]";
    return result;
  }
  case JsonValueType::null: {
    return "null";
  }
  case JsonValueType::none: {
    return "";
  }
  }
  return "";
}

JsonValueType JsonValue::getType() const { return type; }

bool JsonValue::isBool() const { return (type == JsonValueType::boolean); }

bool JsonValue::asBool() const { return *((bool *)data); }

bool JsonValue::isDouble() const { return (type == JsonValueType::decimal); }

double JsonValue::asDouble() const { return *((double *)data); }

bool JsonValue::isInt() const { return (type == JsonValueType::integer); }

int64_t JsonValue::asInt() const { return *((int64_t *)data); }

bool JsonValue::isJson() const { return (type == JsonValueType::json); }

nuo::Json JsonValue::asJson() const { return *((Json *)data); }

bool JsonValue::isList() const { return (type == JsonValueType::list); }

std::vector<JsonValue> JsonValue::asList() const {
  return *((std::vector<JsonValue> *)data);
}

bool JsonValue::isNull() const { return (type == JsonValueType::null); }

bool JsonValue::isNone() const { return (type == JsonValueType::none); }

bool JsonValue::isString() const { return (type == JsonValueType::string); }

std::string JsonValue::asString() const { return *((std::string *)data); }

std::ostream &operator<<(std::ostream &stream, const JsonValue &val) {
  std::operator<<(stream, val.toString());
  return stream;
}

void JsonValue::clear() {
  if (data) {
    switch (type) {
    case JsonValueType::string: {
      delete (std::string *)data;
      break;
    }
    case JsonValueType::integer: {
      delete (int64_t *)data;
      break;
    }
    case JsonValueType::decimal: {
      delete (double *)data;
      break;
    }
    case JsonValueType::boolean: {
      delete (bool *)data;
      break;
    }
    case JsonValueType::json: {
      ((Json *)data)->clear();
      delete (Json *)data;
      break;
    }
    case JsonValueType::list: {
      auto vec = (std::vector<JsonValue> *)data;
      for (auto val : (*vec)) {
        val.clear();
      }
      vec->clear();
      delete vec;
      break;
    }
    case JsonValueType::null:
    case JsonValueType::none:
      break;
    }
    data = nullptr;
  }
}

JsonValue::~JsonValue() noexcept { clear(); }

Json::Json() {}

Json::Json(std::string val) {
  auto parser = JsonParser();
  parser.lex(val);
  auto res = parser.parse();
  keys = std::move(res.keys);
  values = std::move(res.values);
  setLevel(res.level);
  setSpaces(res.spaces);
}

Json::Json(Json const &other)
    : keys(other.keys), values(other.values), level(other.level),
      spaces(other.spaces) {}

Json::Json(Json &&other)
    : keys(other.keys), values(other.values), level(other.level),
      spaces(other.spaces) {
  other.keys.clear();
  other.values.clear();
}

Json &Json::_(std::string key, JsonValue val) {
  if (has(key)) {
    (*this)[key] = val;
  } else {
    keys.push_back(key);
    values.push_back(val);
  }
  return *this;
}

void Json::operator=(Json const &other) {
  keys = other.keys;
  values = other.values;
  level = other.level;
  spaces = other.spaces;
}

void Json::operator=(Json &&other) {
  keys = other.keys;
  values = other.values;
  level = other.level;
  spaces = other.spaces;
  other.keys.clear();
  other.values.clear();
}

void Json::setLevel(unsigned lev) const {
  level = lev;
  for (auto val : values) {
    if (val.type == JsonValueType::json) {
      (((Json *)(val.data)))->setLevel(lev + 1);
    }
  }
}

void Json::setSpaces(unsigned spc) const {
  spaces = spc;
  for (auto val : values) {
    if (val.type == JsonValueType::json) {
      (((Json *)(val.data)))->setSpaces(spc);
    }
  }
}

std::string Json::toString() const {
  if (size() == 0) {
    return "{}";
  } else {
    std::string result("{\n");
    for (std::size_t i = 0; i < keys.size(); i++) {
      if (values.at(i).type == JsonValueType::none) {
        if ((i != (keys.size() - 1)) && (values.at(i + 1))) {
          result += ",\n";
        }
        continue;
      }
      for (std::size_t j = 0; j < (level + 1); j++) {
        for (std::size_t k = 0; k < spaces; k++) {
          result += " ";
        }
      }
      if (values.at(i).type == JsonValueType::json) {
        ((Json *)(values.at(i).data))->setLevel(level + 1);
      }
      result += ('"' + keys.at(i) + '"' + " : " + values.at(i).toString());
      if ((i != (keys.size() - 1)) && (values.at(i + 1))) {
        result += ",\n";
      }
    }
    result += "\n";
    for (std::size_t j = 0; j < level; j++) {
      for (std::size_t k = 0; k < spaces; k++) {
        result += " ";
      }
    }
    result += "}";
    return result;
  }
}

bool Json::has(const std::string key) const {
  for (std::size_t i = 0; i < keys.size(); i++) {
    if (keys.at(i) == key) {
      return true;
    }
  }
  return false;
}

JsonValue &Json::operator[](const std::string key) {
  for (std::size_t i = 0; i < keys.size(); i++) {
    if (keys.at(i) == key) {
      return values[i];
    }
  }
  keys.push_back(key);
  auto none = JsonValue::none();
  values.push_back(none);
  return values[values.size() - 1];
}

bool Json::operator==(const Json &other) const {
  if (size() != other.size()) {
    return false;
  }
  for (std::size_t i = 0; i < keys.size(); i++) {
    if (!values.at(i).isNone()) {
      if (keys.at(i) != other.keys.at(i)) {
        return false;
      }
      if (values.at(i) != other.values.at(i)) {
        return false;
      }
    }
  }
  return true;
}

bool Json::operator!=(const Json &other) const { return !((*this) == other); }

std::size_t Json::size() const {
  std::size_t result = 0;
  for (std::size_t i = 0; i < values.size(); i++) {
    if (values.at(i).type != JsonValueType::none) {
      result++;
    }
  }
  return result;
}

std::ostream &operator<<(std::ostream &os, const Json &json) {
  std::operator<<(os, json.toString());
  return os;
}

void Json::clear() noexcept {
  for (auto val : values) {
    val.clear();
  }
  keys.clear();
  values.clear();
}

Json::~Json() noexcept { clear(); }

} // namespace nuo
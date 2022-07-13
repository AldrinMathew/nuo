#include "nuo/json.hpp"
#include <initializer_list>
#include <iostream>

namespace nuo {

JsonValue::JsonValue(JsonValueType _type, void *_data)
    : data(_data), type(_type) {}

JsonValue JsonValue::none() { return JsonValue(JsonValueType::none, nullptr); }

JsonValue::operator bool() const { return (type != JsonValueType::none); }

JsonValue::JsonValue() : data(nullptr), type(JsonValueType::null) {}

void JsonValue::operator=(const std::nullptr_t ptr) {
  clear();
  type = JsonValueType::null;
  data = nullptr;
}

JsonValue::JsonValue(int val)
    : data(new int64_t(val)), type(JsonValueType::integer) {}

void JsonValue::operator=(const int val) {
  clear();
  type = JsonValueType::integer;
  data = new int64_t(val);
}

JsonValue::JsonValue(int64_t val)
    : data(new int64_t(val)), type(JsonValueType::integer) {}

void JsonValue::operator=(const int64_t val) {
  clear();
  type = JsonValueType::integer;
  data = new int64_t(val);
}

JsonValue::JsonValue(double val)
    : data(new double(val)), type(JsonValueType::decimal) {}

void JsonValue::operator=(const double val) {
  clear();
  type = JsonValueType::decimal;
  data = new double(val);
}

JsonValue::JsonValue(std::string val)
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

JsonValue::JsonValue(bool val)
    : data(new bool(val)), type(JsonValueType::boolean) {}

void JsonValue::operator=(const bool val) {
  clear();
  type = JsonValueType::boolean;
  data = new bool(val);
}

JsonValue::JsonValue(Json val)
    : data(new Json(val)), type(JsonValueType::json) {}

void JsonValue::operator=(Json val) {
  clear();
  type = JsonValueType::json;
  data = new Json(val);
}

JsonValue::JsonValue(Vec<JsonValue> val)
    : data(new Vec<JsonValue>()), type(JsonValueType::list) {
  for (auto elem : val) {
    ((Vec<JsonValue> *)data)->push(elem);
  }
}

void JsonValue::operator=(Vec<JsonValue> val) {
  clear();
  type = JsonValueType::list;
  data = new Vec<JsonValue>(val);
}

JsonValue::JsonValue(std::initializer_list<JsonValue> val)
    : data(new Vec<JsonValue>()), type(JsonValueType::list) {
  for (auto &elem : val) {
    ((Vec<JsonValue> *)data)->push(elem);
  }
}

void JsonValue::operator=(std::initializer_list<JsonValue> val) {
  clear();
  type = JsonValueType::list;
  data = new Vec<JsonValue>();
  for (auto &elem : val) {
    ((Vec<JsonValue> *)data)->push(elem);
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

JsonValue::JsonValue(const JsonValue &other)
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
    auto vals = (Vec<JsonValue> *)other.data;
    data = new Vec<JsonValue>(*vals);
    break;
  }
  case JsonValueType::null:
  case JsonValueType::none: {
    break;
  }
  }
}

void JsonValue::operator=(const JsonValue &other) {
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
    auto vals = (Vec<JsonValue> *)other.data;
    data = new Vec<JsonValue>();
    for (std::size_t i = 0; i < vals->length(); i++) {
      ((Vec<JsonValue> *)data)->push(JsonValue(vals->at(i).get()));
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
    auto otherVals = (Vec<JsonValue> *)other.data;
    auto vals = (Vec<JsonValue> *)data;
    if (otherVals->length() != vals->length()) {
      return false;
    }
    for (std::size_t i = 0; i < vals->length(); i++) {
      if (vals->at(i).get() != otherVals->at(i).get()) {
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
    auto list = (Vec<JsonValue> *)data;
    for (std::size_t i = 0; i < list->length(); i++) {
      result += list->at(i).get().toString();
      if (i != (list->length() - 1)) {
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

bool JsonValue::isDouble() const { return (type == JsonValueType::decimal); }

bool JsonValue::isInt() const { return (type == JsonValueType::integer); }

bool JsonValue::isJson() const { return (type == JsonValueType::json); }

bool JsonValue::isList() const { return (type == JsonValueType::list); }

bool JsonValue::isNull() const { return (type == JsonValueType::null); }

bool JsonValue::isNone() const { return (type == JsonValueType::none); }

bool JsonValue::isString() const { return (type == JsonValueType::string); }

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
      auto vec = (Vec<JsonValue> *)data;
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

Json::Json(const Json &other)
    : keys(other.keys), values(other.values), level(other.level),
      spaces(other.spaces) {}

Json::Json(Json &&other)
    : keys(other.keys), values(other.values), level(other.level),
      spaces(other.spaces) {
  other.keys.clear();
  other.values.clear();
}

Json &Json::_(std::string key, JsonValue val) {
  keys.push(key);
  values.push(val);
  return *this;
}

void Json::operator=(const Json &other) {
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
    for (std::size_t i = 0; i < keys.length(); i++) {
      if (values.at(i).get().type == JsonValueType::none) {
        if ((i != (keys.length() - 1)) && (values.at(i + 1).get())) {
          result += ",\n";
        }
        continue;
      }
      for (std::size_t j = 0; j < (level + 1); j++) {
        for (std::size_t k = 0; k < spaces; k++) {
          result += " ";
        }
      }
      if (values.at(i).get().type == JsonValueType::json) {
        ((Json *)(values.at(i).get().data))->setLevel(level + 1);
      }
      result += ('"' + keys.at(i).get() + '"' + " : " +
                 values.at(i).get().toString());
      if ((i != (keys.length() - 1)) && (values.at(i + 1).get())) {
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
  for (std::size_t i = 0; i < keys.length(); i++) {
    if (keys.at(i).get() == key) {
      return true;
    }
  }
  return false;
}

JsonValue &Json::operator[](const std::string key) {
  for (std::size_t i = 0; i < keys.length(); i++) {
    if (keys.at(i).get() == key) {
      return values[i];
    }
  }
  keys.push(key);
  auto none = JsonValue::none();
  values.push(none);
  return values[values.length() - 1];
}

bool Json::operator==(const Json &other) const {
  if (size() != other.size()) {
    return false;
  }
  for (std::size_t i = 0; i < keys.length(); i++) {
    if (!values.at(i).get().isNone()) {
      if (keys.at(i).get() != other.keys.at(i).get()) {
        return false;
      }
      if (values.at(i).get() != other.values.at(i).get()) {
        return false;
      }
    }
  }
  return true;
}

bool Json::operator!=(const Json &other) const { return !((*this) == other); }

std::size_t Json::size() const {
  std::size_t result = 0;
  for (std::size_t i = 0; i < values.length(); i++) {
    if (values.at(i).get().type != JsonValueType::none) {
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
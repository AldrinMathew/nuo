#include "nuo/json_parser.hpp"
#include "nuo/exception.hpp"
#include "nuo/json.hpp"
#include <optional>
#include <vector>

namespace nuo {

void JsonParser::lex(std::string val) {
  const std::string digits = "0123456789";
  const std::string alpha = "truefalsn";
  for (std::size_t i = 0; i < val.size(); i++) {
    auto chr = val.at(i);
    if (chr == ' ' || chr == '\n' || chr == '\r' || chr == '\t') {
      continue;
    } else if (chr == '{') {
      toks.push_back(Token(TokenType::curlyBraceOpen));
    } else if (chr == '}') {
      toks.push_back(Token(TokenType::curlyBraceClose));
    } else if (chr == '[') {
      toks.push_back(Token(TokenType::bracketOpen));
    } else if (chr == ']') {
      toks.push_back(Token(TokenType::bracketClose));
    } else if (chr == ':') {
      toks.push_back(Token(TokenType::colon));
    } else if (chr == ',') {
      toks.push_back(Token(TokenType::comma));
    } else if (chr == '"') {
      std::string str;
      bool isEscape = false;
      std::size_t j = i + 1;
      for (; (isEscape ? true : val.at(j) != '"') && (j < val.size()); j++) {
        if (isEscape) {
          if (val.at(j) == '"') {
            str += '"';
          } else if (val.at(j) == 'b') {
            str += '\b';
          } else if (val.at(j) == 'f') {
            str += '\f';
          } else if (val.at(j) == 'n') {
            str += '\n';
          } else if (val.at(j) == 't') {
            str += '\t';
          } else if (val.at(j) == '\\') {
            str += "\\";
          } else {
            throw(Exception("Wrong escape character found in json string"));
          }
          isEscape = false;
        } else {
          if (val.at(j) == '\\') {
            isEscape = true;
          } else {
            str += val.at(j);
          }
        }
      }
      i = j;
      toks.push_back(Token(TokenType::string, str));
    } else if ((digits.find(val.at(i)) != std::string::npos) ||
               (val.at(i) == '-')) {
      bool isFloat = false;
      std::string num(val.substr(i, 1));
      std::string decimal;
      std::size_t j = i + 1;
      for (; ((isFloat ? (digits.find(val.at(j)) != std::string::npos)
                       : ((digits.find(val.at(j)) != std::string::npos) ||
                          (val.at(j) == '.'))) &&
              (j < val.size()));
           j++) {
        if (isFloat) {
          decimal += val.at(j);
        } else {
          if (val.at(j) != '.') {
            num += val.at(j);
          } else {
            isFloat = true;
          }
        }
      }
      i = j - 1;
      if (decimal.empty()) {
        isFloat = false;
      } else {
        bool onlyZeroes = true;
        for (auto dc : decimal) {
          if (dc != '0') {
            onlyZeroes = false;
          }
        }
        isFloat = !onlyZeroes;
      }
      if (isFloat) {
        toks.push_back(Token(TokenType::floating, num + '.' + decimal));
      } else {
        toks.push_back(Token(TokenType::integer, num));
      }
    } else if (alpha.find(val.at(i)) != std::string::npos) {
      std::string idt(val.substr(i, 1));
      std::size_t j = i + 1;
      for (; ((alpha.find(val.at(j)) != std::string::npos) && (j < val.size()));
           j++) {
        idt += val.at(j);
      }
      if (idt == "true") {
        toks.push_back(Token(TokenType::True));
      } else if (idt == "false") {
        toks.push_back(Token(TokenType::False));
      } else if (idt == "null") {
        toks.push_back(Token(TokenType::null));
      } else {
        throw Exception("Invalid symbol found `" + idt + "` at " +
                        std::to_string(i));
      }
      i = j - 1;
    } else {
      throw Exception("Invalid symbol found at " + std::to_string(i));
    }
  }
}

bool JsonParser::isNext(TokenType type, std::size_t i = 0) const {
  return (i < toks.size()) ? (toks.at(i + 1).type == type) : false;
}

std::optional<std::size_t>
JsonParser::getPairEnd(const bool isList, const std::size_t from,
                       const std::optional<std::size_t> to) const {
  unsigned collisions = 0;
  for (std::size_t i = from + 1;
       ((to.has_value() ? (i < to.value()) : true) && (i < toks.size())); i++) {
    auto tok = toks.at(i);
    if (isList) {
      if (tok.type == TokenType::bracketOpen) {
        collisions++;
      } else if (tok.type == TokenType::bracketClose) {
        if (collisions > 0) {
          collisions--;
        } else {
          return i;
        }
      }
    } else {
      if (tok.type == TokenType::curlyBraceOpen) {
        collisions++;
      } else if (tok.type == TokenType::curlyBraceClose) {
        if (collisions > 0) {
          collisions--;
        } else {
          return i;
        }
      }
    }
  }
  return std::nullopt;
}

bool JsonParser::hasPrimaryCommas(std::size_t from, std::size_t to) const {
  for (std::size_t i = from + 1; i < to; i++) {
    auto tok = toks.at(i);
    switch (tok.type) {
    case TokenType::curlyBraceOpen:
    case TokenType::bracketOpen: {
      bool isList = (tok.type == TokenType::bracketOpen);
      auto bCloseRes = getPairEnd(isList, i, to);
      if (bCloseRes.has_value()) {
        i = bCloseRes.value();
      } else {
        throw Exception(std::string("End for ") + (isList ? "[" : "{") +
                        " could not be found");
      }
      break;
    }
    case TokenType::comma: {
      return true;
    }
    default: {
      continue;
    }
    }
  }
  return false;
}

std::vector<std::size_t> JsonParser::getPrimaryCommas(std::size_t from,
                                                      std::size_t to) const {
  std::vector<std::size_t> result;
  for (std::size_t i = from + 1; i < to; i++) {
    auto tok = toks.at(i);
    switch (tok.type) {
    case TokenType::curlyBraceOpen:
    case TokenType::bracketOpen: {
      bool isList = (tok.type == TokenType::bracketOpen);
      auto bCloseRes = getPairEnd(isList, i, to);
      if (bCloseRes.has_value()) {
        i = bCloseRes.value();
      } else {
        throw Exception(std::string("End for ") + (isList ? "[" : "{") +
                        " could not be found");
      }
      break;
    }
    case TokenType::comma: {
      result.push_back(i);
      break;
    }
    default: {
      continue;
    }
    }
  }
  return result;
}

JsonValue JsonParser::parseValue(std::size_t from, std::size_t to) const {
  for (std::size_t i = from + 1; i < to; i++) {
    auto &tok = toks.at(i);
    switch (tok.type) {
    case TokenType::True:
    case TokenType::False: {
      return JsonValue(tok.type == TokenType::True);
    }
    case TokenType::curlyBraceOpen: {
      auto bCloseRes = getPairEnd(false, i, to);
      if (bCloseRes.has_value()) {
        return parse(i - 1, bCloseRes.value() + 1);
      } else {
        throw Exception("End for { could not be found for value");
      }
    }
    case TokenType::curlyBraceClose: {
      throw Exception("Invalid } found");
    }
    case TokenType::string: {
      return JsonValue(tok.value);
    }
    case TokenType::integer: {
      return JsonValue(std::stoi(tok.value));
    }
    case TokenType::floating: {
      return JsonValue(std::stod(tok.value));
    }
    case TokenType::comma: {
      throw Exception("Invalid , found");
    }
    case TokenType::colon: {
      throw Exception("Invalid : found");
    }
    case TokenType::null: {
      return JsonValue();
    }
    case TokenType::bracketOpen: {
      auto bCloseRes = getPairEnd(true, i, to);
      if (bCloseRes.has_value()) {
        auto bClose = bCloseRes.value();
        std::vector<JsonValue> vals;
        if (hasPrimaryCommas(i, bClose)) {
          auto sepPos = getPrimaryCommas(i, bClose);
          vals.push_back(parseValue(i, sepPos.front()));
          for (std::size_t j = 0; j < (sepPos.size() - 1); j++) {
            auto res = parseValue(sepPos.at(j), sepPos.at(j + 1));
            vals.push_back(res);
          }
          vals.push_back(parseValue(sepPos.back(), bClose));
        } else if (i != bClose) {
          vals.push_back(parseValue(i, bClose));
        }
        return JsonValue(vals);
      } else {
        throw Exception("End for [ could not be found for value");
      }
    }
    case TokenType::bracketClose:
      throw Exception("Invalid ] found");
    }
  }
  return JsonValue::none();
}

std::vector<std::pair<std::string, JsonValue>>
JsonParser::parsePairs(std::size_t from, std::size_t to) const {
  std::vector<std::pair<std::string, JsonValue>> result;
  for (std::size_t i = from + 1; i < to; i++) {
    auto tok = toks.at(i);
    if (tok.type == TokenType::string) {
      if (isNext(TokenType::colon, i)) {
        switch (toks.at(i + 2).type) {
        case TokenType::True:
        case TokenType::False: {
          if (isNext(TokenType::comma, i + 2) ||
              isNext(TokenType::curlyBraceClose, i + 2)) {
            result.push_back(std::pair(tok.value, parseValue(i + 1, i + 3)));
            i += 2;
            break;
          } else {
            throw Exception("Invalid token found after boolean");
          }
        }
        case TokenType::curlyBraceOpen: {
          auto bCloseRes = getPairEnd(false, i + 2, to);
          if (bCloseRes.has_value()) {
            auto bClose = bCloseRes.value();
            if (isNext(TokenType::comma, bClose) ||
                isNext(TokenType::curlyBraceClose, bClose)) {
              result.push_back(std::pair(tok.value, parse(i + 1, bClose + 1)));
              i = bClose;
              break;
            } else {
              throw Exception("Invalid token found after Json object");
            }
          } else {
            throw Exception("End for { could not be found while parsing pairs");
          }
        }
        case TokenType::curlyBraceClose: {
          throw Exception("Invalid } found");
        }
        case TokenType::string: {
          if (isNext(TokenType::comma, i + 2) ||
              isNext(TokenType::curlyBraceClose, i + 2)) {
            result.push_back(std::pair(tok.value, parseValue(i + 1, i + 3)));
            i += 2;
            break;
          } else {
            throw Exception("Invalid token found after string");
          }
        }
        case TokenType::integer: {
          if (isNext(TokenType::comma, i + 2) ||
              isNext(TokenType::curlyBraceClose, i + 2)) {
            result.push_back(std::pair(tok.value, parseValue(i + 1, i + 3)));
            i += 2;
            break;
          } else {
            throw Exception("Invalid token found after integer");
          }
        }
        case TokenType::floating: {
          if (isNext(TokenType::comma, i + 2) ||
              isNext(TokenType::curlyBraceClose, i + 2)) {
            result.push_back(std::pair(tok.value, parseValue(i + 1, i + 3)));
            i += 2;
            break;
          } else {
            throw Exception("Invalid token found after floating point number");
          }
        }
        case TokenType::bracketOpen: {
          auto bCloseRes = getPairEnd(true, i + 2, to);
          if (bCloseRes.has_value()) {
            auto bClose = bCloseRes.value();
            if (isNext(TokenType::comma, bClose) ||
                isNext(TokenType::curlyBraceClose, bClose)) {
              result.push_back(
                  std::pair(tok.value, parseValue(i + 1, bClose + 1)));
              i = bClose;
              break;
            } else {
              throw Exception("Invalid token found after list");
            }
          } else {
            throw Exception("End for [ could not be found while parsing pair");
          }
        }
        case TokenType::null: {
          if (isNext(TokenType::comma, i + 2) ||
              isNext(TokenType::curlyBraceClose, i + 2)) {
            result.push_back(std::pair(tok.value, JsonValue()));
            i += 2;
            break;
          } else {
            throw Exception("Invalid token found after null");
          }
        }
        case TokenType::comma: {
          throw Exception("Invalid , found");
        }
        case TokenType::colon: {
          throw Exception("Invalid : found");
        }
        case TokenType::bracketClose:
          throw Exception("Invalid ] found");
        }
      } else {
        Exception("Colon expected after the key");
      }
    } else if (tok.type == TokenType::comma) {
      if (!isNext(TokenType::string, i)) {
        throw Exception("Trailing commas are not allowed. Expected a key "
                        "after the comma");
      }
    } else {
      throw Exception("Illegal token found inside Json scope");
    }
  }
  return result;
}

Json JsonParser::parse(std::size_t from, std::size_t to) const {
  auto result = Json();
  if (to == -1) {
    to = toks.size();
  }
  for (std::size_t i = from + 1; i < to; i++) {
    auto tok = toks.at(i);
    switch (tok.type) {
    case TokenType::False: {
      throw Exception("false should not occur outside Json scope");
      break;
    }
    case TokenType::True: {
      throw Exception("true should not occur outside Json scope");
      break;
    }
    case TokenType::curlyBraceOpen: {
      auto bClose = getPairEnd(false, i, to);
      if (bClose.has_value()) {
        auto pairs = parsePairs(i, bClose.value());
        for (auto &pair : pairs) {
          result[pair.first] = pair.second;
        }
        i = bClose.value();
      } else {
        throw Exception("End for { could not be found");
      }
      break;
    }
    case TokenType::curlyBraceClose: {
      break;
    }
    case TokenType::string: {
      throw Exception("String should not occur outside Json scope");
      break;
    }
    case TokenType::integer: {
      throw Exception("Integer should not occur outside Json scope");
      break;
    }
    case TokenType::floating: {
      throw Exception("Float number should not occur outside Json scope");
      break;
    }
    case TokenType::comma: {
      throw Exception("Comma should not occur outside Json scope");
      break;
    }
    case TokenType::colon: {
      throw Exception("Colon should not occur outside Json scope");
      break;
    }
    case TokenType::null: {
      throw Exception("Null should not occur outside Json scope");
      break;
    }
    case TokenType::bracketOpen: {
      throw Exception("List should not begin outside Json scope");
      break;
    }
    case TokenType::bracketClose: {
      throw Exception("] should not occur outside Json scope");
      break;
    }
    }
  }

  return result;
}

} // namespace nuo
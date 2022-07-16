#ifndef NUO_JSON_PARSER_HPP
#define NUO_JSON_PARSER_HPP

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace nuo {

class Json;
class JsonValue;

class JsonParser {
private:
  enum class TokenType {
    False,
    True,
    curlyBraceOpen,
    curlyBraceClose,
    string,
    integer,
    floating,
    comma,
    colon,
    null,
    bracketOpen,
    bracketClose,
  };

  class Token {
  public:
    Token(TokenType _type) : type(_type), value() {}
    Token(TokenType _type, std::string _val) : type(_type), value(_val) {}

    TokenType type;
    std::string value;
  };

  std::vector<Token> toks;

protected:
  friend class Json;

  void lex(std::string val);

  Json parse(std::size_t from = -1, std::size_t to = -1) const;

  JsonValue parseValue(std::size_t from, std::size_t to) const;

  std::vector<std::pair<std::string, JsonValue>>
  parsePairs(std::size_t from, std::size_t to) const;

  std::optional<std::size_t>
  getPairEnd(const bool isList, const std::size_t from,
             const std::optional<std::size_t> to) const;

  bool isNext(TokenType type, std::size_t current) const;

  bool hasPrimaryCommas(std::size_t from, std::size_t to) const;

  std::vector<std::size_t> getPrimaryCommas(std::size_t from,
                                            std::size_t to) const;
};

} // namespace nuo

#endif
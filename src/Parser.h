#include "Lexer.h"

#include <string>
#include <vector>

class Rule {
public:
  TokenTag head_;
  std::vector<TokenTag> form_;

  Rule(TokenTag head = DEFAULT, std::vector<TokenTag> form = {}) {
    head_ = head;
    form_ = form;
  }
};

class ParseTree {
public:
  TokenTag head_;
  std::vector<ParseTree *> sons_;
  std::string content_;

  ParseTree(TokenTag head = DEFAULT, std::vector<ParseTree *> sons = {}, std::string content = "") {
    head_ = head;
    sons_ = sons;
    content_ = content;
  }
};

class Parser {
public:
  Parser() {}
  ~Parser() {}
  virtual std::string GetParseTree(const std::vector<Token> &tokens, ParseTree *parse_tree);
};

class ManualParser : public Parser {
public:
  ManualParser();
  ~ManualParser();
  virtual std::string GetParseTree(const std::vector<Token> &tokens, ParseTree *parse_tree) final;
};
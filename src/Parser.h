#include "Lexer.h"

#include <string>
#include <vector>

class Rule {
public:
  TokenTag result_;
  NFANode *head_;
  Rule(TokenTag result = TokenTag::DEFAULT, NFANode *head) {
    result_ = result;
    head_ = head;
  }
  bool operator < (const Rule &b) const {
    return head_ < b.head_;
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
  virtual std::string GetParseTree(const std::vector<Token> &tokens, ParseTree* &parse_tree) {
    return "";
  }

};

class ManualParser : public Parser {
public:
  ManualParser();
  ~ManualParser();
  std::string GetParseTree(const std::vector<Token> &tokens, ParseTree* &parse_tree) final;
  void AddRule(TokenTag head, std::vector<TokenTag> form);

  std::vector<Rule> rules_;
};
#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>

enum TokenTag {
  ID,
  INT,
  BOOL,
  RESERVED,
  OP,
  SYMBOL,
  COMMENT,
  DEFAULT
};

struct Token {
  TokenTag tag;
  std::string chars;
  Token(TokenTag _tag = DEFAULT, std::string _chars = "") : tag(_tag), chars(_chars) {}
};

class NFANode {
public: 
  NFANode *nex_[256];
  bool valid_;
  TokenTag tag_;
  
  NFANode(bool valid = false, TokenTag tag = DEFAULT) {
    memset(nex_, 0, 256 * sizeof(NFANode *));
    valid_ = valid;
    tag_ = tag;
  }
};

class Lexer {
public:
  Lexer() {}
  virtual std::string GetTokens(std::ifstream &in_stream, std::vector<Token> &tokens) {
    std::cout << "I am the base. " << std::endl;
    return std::string("");
  }
};

class ManualLexer : public Lexer {
public:
  ManualLexer();
  std::string GetTokens(std::ifstream &in_stream, std::vector<Token> &tokens) final;

  std::vector<NFANode *> heads_;
};

class ANTLRLexer : public Lexer {
public:
  ANTLRLexer();
  std::string GetTokens(std::ifstream &in_stream, std::vector<Token> &tokens) final;
};
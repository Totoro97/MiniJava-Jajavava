#pragma once
#include <iostream>
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
  DEFAULT
};

struct Token {
  TokenTag tag;
  std::string chars;
};

class NFANode {
public: 
  NFANode *nex_[256];
  bool valid_;
  TokenTag tag_;
  
  NFANode(bool valid = false, TokenTag tag = DEFAULT) {
    valid_ = valid;
    tag_ = tag;
  }
};

class Lexer {
public:
  Lexer() {}
  virtual std::string GetTokens(std::ifstream &in_stream, std::vector<Token> &tokens) {
    std::cout << "I am the base. " << std::endl;
  }
};

class ManualLexer : public Lexer {
public:
  ManualLexer();
  std::string GetTokens(std::ifstream &in_stream, std::vector<Token> &tokens);

  std::vector<NFANode *> heads_;
};

class ANTLRLexer : public Lexer {
public:
  ANTLRLexer();
  std::string GetTokens(std::ifstream &in_stream, std::vector<Token> &tokens);
};
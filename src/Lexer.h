#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

enum TokenTag {
  ID,
  INT,
  RESERVED,
  OP
};

struct Token {
  TokenTag tag;
  std::string chars;
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
};

class ANTLRLexer : public Lexer {
public:
  ANTLRLexer();
  std::string GetTokens(std::ifstream &in_stream, std::vector<Token> &tokens);
};
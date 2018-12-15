#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>

enum TokenTag {
  ID, INT, BOOLEAN, BIOP, COMMENT, TRUE,  FALSE,
  CLASS, PUBLIC, STATIC, VOID, MAIN, STRING, EXTENDS, LENGTH,
  RETURN, IF, ELSE, WHILE, PRINT, THIS, NEW, COMMA, SEMI, LCUR, RCUR, LSQR, RSQR,
  LB, RB, DOT, PLUS, MINUS, EQ, NT, INT_LITERAL,

  GOAL, MAIN_CLASS, CLASS_DECLARATION, VAR_DECLARATION,
  METHOD_DECLARATION, TYPE, STATEMENT,
  EXPRESSION, EXTENDS_IDENTIFIER,

  TYPE_ID_MANY, DOT_TYPE_ID, ONE_OR_ZERO, MANY_OR_ZERO,

  PARAS, DOT_EXPRESSION,

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
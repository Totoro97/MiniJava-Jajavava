#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>

enum TokenTag {
  ID, INT, BOOLEAN, BIOP, COMMENT, TRUE,  FALSE,
  CLASS, PUBLIC, STATIC, VOID, MAIN, STRING, EXTENDS,
  RETURN, IF, ELSE, PRINT, THIS, NOW, COMMA, SEMI, LCUR, RCUR, LSQR, RSQR,
  LB, RB, DOT, PLUS, MINUS, EQ, NT, INT_LITERAL,

  GOAL, MAIN_CLASS, CALSS_DECLARATION, CLASS_DECLARATIONS, VAR_RECLARATION, VAR_DECLARATIONS,
  MATHOD_DECLARATION, METHOD_DECLARATIONS, TYPE, STATEMENT, STATEMENTS,
  EXPRESSION, COMMA_EXPRESSIONS, IDENTIFIER,

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
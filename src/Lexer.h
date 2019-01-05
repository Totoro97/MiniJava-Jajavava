#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>

enum TokenTag {
  DEFAULT,

  ID, INT, BOOLEAN, BIOP, COMMENT, TRUE, FALSE,
  CLASS, PUBLIC, STATIC, VOID, MAIN, STRING, EXTENDS, LENGTH,
  RETURN, IF, ELSE, WHILE, PRINT, THIS, NEW, COMMA, SEMI, LCUR, RCUR, LSQR, RSQR,
  LB, RB, DOT, PLUS, MINUS, EQ, NT, INT_LITERAL,

  GOAL, MAIN_CLASS, CLASS_DECLARATION, VAR_DECLARATION,
  METHOD_DECLARATION, TYPE, STATEMENT,
  EXPRESSION, EXTENDS_IDENTIFIER,

  TYPE_ID_MANY, COMMA_TYPE_ID, ONE_OR_ZERO, MANY_OR_ZERO,

  PARAS, COMMA_EXPRESSION,

  STATEMENT_S, STATEMENT_IF_ELSE, STATEMENT_WHILE, STATEMENT_PRINT, STATEMENT_EQ, STATEMENT_ARRAY,
  
  EXPRESSION_BIOP, EXPRESSION_INDEX, EXPRESSION_LENGTH, EXPRESSION_FUNCTION, EXPRESSION_NEW_ARRAY,
  EXPRESSION_NEW_CLASS, EXPRESSION_NT, EXPRESSION_BRACKET,
  EXPRESSION_INT, EXPRESSION_BOOL, EXPRESSION_ID, EXPRESSION_THIS,
  TYPE_INT, TYPE_BOOL, TYPE_ARRAY, TYPE_CLASS,

  END
};

static const std::vector<std::string> token2str = {
  "DEFAULT",

  "ID", "INT", "BOOLEAN", "BIOP", "COMMENT", "TRUE", "FALSE",
  "CLASS", "PUBLIC", "STATIC", "VOID", "MAIN", "STRING", "EXTENDS", "LENGTH",
  "RETURN", "IF", "ELSE", "WHILE", "PRINT", "THIS", "NEW", "COMMA", "SEMI", "LCUR", "RCUR", "LSQR", "RSQR",
  "LB", "RB", "DOT", "PLUS", "MINUS", "EQ", "NT", "INT_LITERAL",

  "GOAL", "MAIN_CLASS", "CLASS_DECLARATION", "VAR_DECLARATION",
  "METHOD_DECLARATION", "TYPE", "STATEMENT",
  "EXPRESSION", "EXTENDS_IDENTIFIER",

  "TYPE_ID_MANY", "COMMA_TYPE_ID", "ONE_OR_ZERO", "MANY_OR_ZERO",

  "PARAS", "COMMA_EXPRESSION",

  "STATEMENT_S", "STATEMENT_IF_ELSE", "STATEMENT_WHILE", "STATEMENT_PRINT", "STATEMENT_EQ", "STATEMENT_ARRAY",
  "EXPRESSION_BIOP", "EXPRESSION_INDEX", "EXPRESSION_LENGTH", "EXPRESSION_FUNCTION", "EXPRESSION_NEW_ARRAY",
  "EXPRESSION_NEW_CLASS", "EXPRESSION_NT", "EXPRESSION_BRACKET",
  "EXPRESSION_INT", "EXPRESSION_BOOL", "EXPRESSION_ID", "EXPRESSION_THIS",
  "TYPE_INT", "TYPE_BOOL", "TYPE_ARRAY", "TYPE_CLASS",

  "END"
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

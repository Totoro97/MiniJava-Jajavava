#pragma once
#include "Lexer.h"
#include "Parser.h"
// std
#include <string>

enum SymbolType {
  SYM_VARIABLE, SYM_FUNCTION, SYM_CLASS, SYM_ARRAY
};

enum DataType {
  DATA_INT, DATA_BOOL
};

class Symbol {
  std::string name;
  SymbolType symbol_type;
  ParseTree *def_pos;
  void *data;

  Symbol(std::string name = "", SymbolType symbol_type = SYM_VARIABLE,
          ParseTree *def_pos = nullptr, void *data = nullptr) :
          name(name), symbol_type(symbol_type), def_pos(def_pos), data(data) {}
};

class SymbolTable {
public:
  SymbolTable() {}
  std::map<std::string, Symbol> table_;
};

// Data Class ---------------------------------------------------------------------------------------------

class DataVariable {
  DataVariable(DataType type = DATA_INT, int data = 0) : type_(type), data_(data) {}
  DataType type_;
  int data_;
};

typedef std::vector<std::pair<DataType, std::string> > Paras;

class DataFunction {
  DataFunction(DataType ret_type = DATA_INT, Paras paras = {}, ParseTree *def_pos = nullptr) :
    ret_type_(ret_type), paras_(paras), def_pos_(def_pos) {}
  DataType ret_type_;
  Paras paras_;
  ParseTree *def_pos_;
};

class DataClass {
  DataClass() {}
  SymbolTable members_;
};

// interpreter --------------------------------------------------------------------------------------------

class Interpreter {
public:
  Interpreter(ParseTree *abstract_parse_tree = nullptr);
  std::string Interprete();
  void GenerateGlobalSymbolTable();
  DataClass* Interpreter::GetDataClassFromParseTree(ParseTree *tree);

  ParseTree *tree_root_;
};

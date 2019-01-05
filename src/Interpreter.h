#pragma once
#include "Lexer.h"
#include "Parser.h"
// std
#include <iostream>
#include <string>

enum SymbolType {
  SYM_FUNCTION, SYM_CLASS
};

enum DataType {
  DATA_INT, DATA_BOOL, DATA_CLASS, DATA_ARRAY
};

enum ClassType {
  CLASS_INT, CLASS_BOOL, CLASS_CLASS, CLASS_ARRAY
};

class Symbol {
public:
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

class BaseClass {
public:
  BaseClass() {}
  virtual void Assign(const BaseClass &r_value);
  
  ClassType class_type_;
};

class IntClass : BaseClass {
public:
  IntClass(int data = 0) : data_(data) {
    class_type_ = CLASS_INT;
  } 
  void Assign(const BaseClass &r_value) final {
    if (r_value.class_type_ != CLASS_INT) {
      std::cout << "Error: Data Type Not Corrospond." << std::endl;
      exit(0);
    }
    data_ = ((IntClass *) (&r_value))->data_;
  }

  int data_;
};

class BoolClass : BaseClass {
public:
  BoolClass(bool data = false) : data_(data) {
    class_type_ = CLASS_BOOL;
  }
  void Assign(const BaseClass &r_value) final {
    if (r_value.class_type_ != CLASS_BOOL) {
      std::cout << "Error: Data Type Not Corrospond." << std::endl;
      exit(0);
    }
    data_ = ((BoolClass *) (&r_value))->data_;
  }

  bool data_;
};

class ClassClass : BaseClass {
public:
  ClassClass();
  ClassClass(SymbolTable members) {
    members_.LoadFromAnotherSymbolTable(members);
  }

  void Assign(const BaseClass &r_value) final {
    if (r_value.class_type_ != CLASS_CLASS) {
      std::cout << "Error: Data Type Not Corrospond." << std::endl;
      exit(0);
    }
    auto ptr = (ClassClass *) (&r_value);
    if (ptr->class_name_ != class_name_) {
      std::cout << "Error: Data Type Not Corrospond." << std::endl;
      exit(0);
    }
    members_.LoadFromAnotherSymbolTable(ptr->members_);
  }

  SymbolTable members_;
  std::string class_name_ = "";
};

class ArrayClass : BaseClass {
public:
  ArrayClass(int length = 1) {
    data_ = new int[length];
  }
  void Assign(const BaseClass &r_value) final {
    if (r_value.class_type_ != CLASS_ARRAY) {
      std::cout << "Error: Data Type Not Corrospond." << std::endl;
      exit(0);
      data_ = ((ArrayClass *) (&r_value))->data_;
    }
  }
  int *data_;
};

typedef std::vector<std::pair<DataType, std::string> > Paras;

class Function {
public:
  
  DataFunction(DataType ret_type = DATA_INT, Paras paras = {}, ParseTree *def_pos = nullptr) :
    ret_type_(ret_type), paras_(paras), def_pos_(def_pos) {}
  DataType ret_type_;
  Paras paras_;
  ParseTree *def_pos_;
};

// interpreter --------------------------------------------------------------------------------------------

class Interpreter {
public:
  // method
  Interpreter(ParseTree *abstract_parse_tree = nullptr);
  std::string Interprete();
  void GenerateGlobalSymbolTable();
  DataClass* Interpreter::GetDataClassFromParseTree(ParseTree *tree);
  void AddSymbolFromVarDeclaration(ParseTree *tree, SymbolTable *symbol_table);
  void AddSymbolFromMethodDeclaration(ParseTree *tree, SymbolTable *symbol_table);
  // data
  ParseTree *tree_root_;
};

#pragma once
#include "Lexer.h"
#include "Parser.h"
// std
#include <iostream>
#include <stack>
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

// Data Class ---------------------------------------------------------------------------------------------

class BaseClass {
public:
  BaseClass() {}
  virtual void Assign(BaseClass *r_value);
  
  ClassType class_type_;
};

class IntClass : BaseClass {
public:
  IntClass(int data = 0) : data_(data) {
    class_type_ = CLASS_INT;
  } 
  void Assign(BaseClass *r_value) final {
    if (r_value->class_type_ != CLASS_INT) {
      std::cout << "Error: Data Type Not Correspond." << std::endl;
      exit(0);
    }
    data_ = ((IntClass *) (r_value))->data_;
  }

  int data_;
};

class BoolClass : BaseClass {
public:
  BoolClass(bool data = false) : data_(data) {
    class_type_ = CLASS_BOOL;
  }
  void Assign(BaseClass *r_value) final {
    if (r_value->class_type_ != CLASS_BOOL) {
      std::cout << "Error: Data Type Not Correspond." << std::endl;
      exit(0);
    }
    data_ = ((BoolClass *) (r_value))->data_;
  }

  bool data_;
};

class ArrayClass : BaseClass {
public:
  ArrayClass(int length = 1) {
    data_ = new int[length];
    length_ = length;
  }
  void Assign(BaseClass *r_value) final {
    if (r_value->class_type_ != CLASS_ARRAY) {
      std::cout << "Error: Data Type Not Correspond." << std::endl;
      exit(0);
      auto array_ptr = (ArrayClass *) r_value;
      length_ = array_ptr->length_;
      data_ = new int[length_];
      for (int i = 0; i < length_; i++)
        data_[i] = array_ptr->data_[i];
    }
  }
  int *data_ = nullptr;
  int length_ = 0;
};

class Function {
public:
  
  Function(ClassType ret_type = CLASS_INT, Paras paras = {}, ParseTree *def_pos = nullptr) :
    ret_type_(ret_type), paras_(paras), def_pos_(def_pos) {}
  ClassType ret_type_;
  Paras paras_;
  ParseTree *def_pos_;

  BaseClass* Execute(std::vector<BaseClass *> inputs, SymbolTable &symbols);
};

class ClassClass : BaseClass {
public:
  ClassClass(std::string class_name = "") {
    class_name_ = class_name;
  }

  void Assign(BaseClass *r_value) final {
    if (r_value->class_type_ != CLASS_CLASS) {
      std::cout << "Error: Data Type Not Correspond." << std::endl;
      exit(0);
    }
    auto ptr = (ClassClass *) (r_value);
    if (ptr->class_name_ != class_name_) {
      std::cout << "Error: Data Type Not Correspond." << std::endl;
      exit(0);
    }
    members_.LoadFromAnotherSymbolTable(ptr->members_);
    functions_.clear();
    for (auto func : ptr->functions_) {
      functions_.insert(func);
    }
    initialized_ = true;
  }

  SymbolTable members_;
  std::map<std::string, Function *> functions_;
  std::string class_name_ = "";
  bool initialized_ = false;
};

typedef std::vector<std::pair<ClassType, std::string> > Paras;

// SymbolTable --------------------------------------------------------------------------------------------

class SymbolTable {
public:
  SymbolTable() {}
  std::map<std::string, std::stack<BaseClass*>* > table_;

  void Add(std::string id, BaseClass *class_ptr);
  void Change(std::string id, BaseClass *class_ptr);
  void Del(std::string id);
  BaseClass *Find(std::string id);
  BaseClass *DeepCopyFrom(std::string id);
  void LoadFromAnotherSymbolTable(SymbolTable &new_table);
};

// interpreter --------------------------------------------------------------------------------------------

class Interpreter {
public:
  // method
  Interpreter(ParseTree *abstract_parse_tree = nullptr);
  std::string Interprete();
  void GenerateGlobalSymbolTable();
  
  void AddVarDeclaration(ParseTree *tree, SymbolTable &symbols);
  void DelVarDeclaration(ParseTree *tree, SymbolTable &symbols);
  
  void ExecuteStatement(ParseTree *tree, SymbolTable &symbols);
  BaseClass *EvalExpression(ParseTree *tree, SymbolTable &symbols);

  void PrintData(BaseClass *data);

  // data
  SymbolTable class_table_;
};

static Interpreter global_interpreter;
//
// Created by Aska on 2019/1/4.
//

#include "Interpreter.h"
// std

Interpreter::Interpreter(ParseTree *abstract_parse_tree) {
  tree_root_ = abstract_parse_tree;
}

std::string Interpreter::Interprete() {
  GenerateGlobalSymbolTable();
}

void Interpreter::GenerateGlobalSymbolTable() {
  for (auto son : tree_root_->sons_) {
    if (son->head_ != CLASS_DECLARATION) {
      continue;
    }
    auto class_ptr = GetDataClassFromParseTree(son);
  }
}

DataClass* Interpreter::GetDataClassFromParseTree(ParseTree *tree) {
  auto new_class = new DataClass();
  for (auto son : tree->sons_) {
    if (son->head_ == VAR_DECLARATION) {
      // add vars;
      AddSymbolFromVarDeclaration(son, &(new_class->members_));
    }
    else if (son->head_ == METHOD_DECLARATION) {
      AddSymbolFromMethodDeclaration(son, &(new_class->members_));
    }
  }
  return new_class;
}

void Interpreter::AddSymbolFromVarDeclaration(ParseTree *tree, SymbolTable *symbol_table) {
  std::string type_name = tree->sons_[0]->sons_[0]->content_;
  std::string id = tree->sons_[1]->content_;
  if (type_name == "boolean") {
    symbol_table->Add(Symbol(id, SYM_VARIABLE, tree, (void *) new DataVariable(DATA_BOOL)));
  }
  else if (type_name == "int" && (tree->sons_[0]->sons_).size() > 1) {
    symbol_table->Add(Symbol(id, SYM_ARRAY, tree, (void *) new DataArray(0)));
  }
  else if (type_name == "int") {
    symbol_table->Add(Symbol(id, SYM_VARIABLE, tree, (void *) new DataVariable(DATA_INT)));
  }
  else {
    symbol_table->Add(Symbol(id, SYM_CLASS, tree, nullptr));
  }
}

void Interpreter::AddSymbolFromMethodDeclaration(ParseTree *tree, SymbolTable *symbol_table) {
  std::string type_name = tree->sons_[0]->sons_[0]->content_;
  DataType data_type;
  if (type_name == "boolean") {
    data_type = DATA_INT;
  }
  else if (type_name == "int" && (tree->sons_[0]->sons_).size() > 1) {
    data_type = DATA_ARRAY;
  }
  else if (type_name == "int") {
    symbol_table->Add(Symbol(id, SYM_VARIABLE, tree, (void *) new DataVariable(DATA_INT)));
  }
  else {
    symbol_table->Add(Symbol(id, SYM_CLASS, tree, nullptr));
  }
}
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
      
    }
    else if (son->head_ == METHOD_DECLARATION) {

    }
  }
  return new_class;
}
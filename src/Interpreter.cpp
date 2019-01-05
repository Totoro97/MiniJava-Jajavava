//
// Created by Aska on 2019/1/4.
//

#include "Interpreter.h"
// std

// --------------------------- Function ----------------------------------------------

BaseClass* Function::Execute(std::vector<BaseClass *> inputs, SymbolTable &symbols) {
  if (inputs.size() != paras_.size()) {
    std::cout << "Error: Parameters not Correspond" << std::endl;
    exit(0);
  }
  for (int i = 0; i < (int) paras_.size(); i++) {
    if (inputs[i]->class_type_ != paras_[i].first) {
      std::cout << "Error: Parameters not Correspond" << std::endl;
      exit(0);
    }
  }
  for (int i = 0; i < (int) paras_.size(); i++) {
    symbols.Add(paras_[i].first, *(inputs[i]));
  }
  for (auto son : def_pos_->sons_) {
    if (son->head_ == VAR_DECLARATION) {
      global_interpreter.AddVarDeclaration(son, symbols);
    }
  }
  
  // execute
  for (auto son : def_pos_->sons_) {
    if (son->head_ >= STATEMENT_S && son->head_ <= STATEMENT_ARRAY) {
      global_interpreter.ExecuteStatement(son, symbols);
    }
  }
  BaseClass *ret = nullptr;
  for (auto iter = def_pos_->sons_.begin();  iter != def_pos_->sons_.end(); iter++) {
    if ((*iter)->head_ == RETURN) {
      auto exp_ptr = std::next(iter);
      ret = global_interpreter.EvalExpression(*exp_ptr, symbols);
    }
  }
  
  // recover
  for (auto iter = def_pos_->sons_.rbegin(); iter != def_pos_->sons_.rend(); iter++) {
    auto son = *iter;
    if (son->head_ == VAR_DECLARATION) {
      global_interpreter.DelVarDeclaration(son, symbols);
    }
  }
  for (int i = 0; i < (int) paras_.size(); i++) {
    symbols.del(paras_[i].first);
  }
  return ret;
}

// ---------------------- Interpreter ------------------------------------------------
Interpreter::Interpreter(ParseTree *abstract_parse_tree) {
  tree_root_ = abstract_parse_tree;
}

std::string Interpreter::Interprete() {
  GenerateGlobalSymbolTable();
}

void Interpreter::AddVarDeclaration(ParseTree *tree, SymbolTable &symbols) {
  if (tree->head_ != VAR_DECLARATION) {
    return;
  }

  std::string type_name = tree->sons_[0]->sons_[0]->content_;
  
}

void Interpreter::DelVarDeclaration(ParseTree *tree, SymbolTable &symbols) {

}
  
void Interpreter::ExecuteStatement(ParseTree *tree, SymbolTable &symbols) {

}

BaseClass* Interpreter::EvalExpression(ParseTree *tree, const SymbolTable &symbols) {

}


/* -------- deprecated ------------------

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

*/
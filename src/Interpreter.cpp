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
    symbols.Add(paras_[i].second, inputs[i]);
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
    symbols.Del(paras_[i].second);
  }
  return ret;
}

// ---------------------- SymbolTable --------------------------------------------------

// ---------------------- Interpreter --------------------------------------------------

std::string Interpreter::Interprete() {
  GenerateGlobalSymbolTable();
}

void Interpreter::AddVarDeclaration(ParseTree *tree, SymbolTable &symbols) {
  if (tree->head_ != VAR_DECLARATION) {
    return;
  }
  auto type_node = tree->sons_[0];
  std::string id = tree->sons_[1]->content_;
  if (type_node->head_ == TYPE_INT) {
    symbols.Add(id, (BaseClass *) new IntClass());
  }
  else if (type_node->head_ == TYPE_BOOL) {
    symbols.Add(id, (BaseClass *) new BoolClass());
  }
  else if (type_node->head_ == TYPE_ARRAY) {
    symbols.Add(id, (BaseClass *) new ArrayClass());
  }
  else if (type_node->head_ == TYPE_CLASS) {
    ClassClass *class_ptr =
      (ClassClass *) global_interpreter.class_table_.DeepCopyFrom(type_node->sons_[0]->content_);
    if (class_ptr == nullptr) {
      std::cout << "Error: No Such Class." << std::endl;
      exit(0);
    }
    symbols.Add(id, (BaseClass *) class_ptr);
  }
  else {
    std::cout << "Fxxk Your Type" << std::endl;
    exit(0);
  }
  return;
}

void Interpreter::DelVarDeclaration(ParseTree *tree, SymbolTable &symbols) {
  if (tree->head_ != VAR_DECLARATION) {
    return;
  }
  auto type_node = tree->sons_[0];
  std::string id = tree->sons_[1]->content_;
  if (type_node->head_ == TYPE_INT) {
    symbols.Del(id);
  }
  return;
}

void Interpreter::ExecuteStatement(ParseTree *tree, SymbolTable &symbols) {
  if (tree->head_ == STATEMENT_S) {
    for (auto son : tree->sons_) {
      ExecuteStatement(son, symbols);
    }
  }
  else if (tree->head_ == STATEMENT_IF_ELSE) {
    BaseClass *tmp_exp = EvalExpression(tree->sons_[0], symbols);
    if (tmp_exp->class_type_ != CLASS_BOOL) {
      std::cout << "Data Type Invalid" << std::endl;
      exit(0);
    }
    BoolClass *judge_exp = (BoolClass *) tmp_exp;
    if (judge_exp->data_ == true) {
      ExecuteStatement(tree->sons_[1], symbols);
    }
    else {
      ExecuteStatement(tree->sons_[2], symbols);
    }
  }
  else if (tree->head_ == STATEMENT_WHILE) {
    do {
      BaseClass *tmp_exp = EvalExpression(tree->sons_[0], symbols);
      if (tmp_exp->class_type_ != CLASS_BOOL) {
        std::cout << "Data Type Invalid" << std::endl;
        exit(0);
      }
      BoolClass *judge_exp = (BoolClass *) tmp_exp;
      if (judge_exp->data_ == false) {
        break;
      }
      ExecuteStatement(tree->sons_[1], symbols);
    } while (true);
  }
  else if (tree->head_ == STATEMENT_PRINT) {
    PrintData(EvalExpression(tree->sons_[0], symbols));
  }
  else if (tree->head_ == STATEMENT_EQ) {
    BaseClass *tmp_exp = EvalExpression(tree->sons_[1], symbols);
    if (tree->sons_[0]->head_ != ID) {
      std::cout << "Error" << std::endl;
      exit(0);
    }
    symbols.Change(tree->sons_[0]->content_, tmp_exp);
  }
  else if (tree->head_ == STATEMENT_ARRAY) {
    if (tree->sons_[0]->head_ != ID) {
      std::cout << "Error" << std::endl;
      exit(0);
    }
    std::string id = tree->sons_[0]->content_;
    BaseClass *tmp_exp = symbols.Find(id);
    if (tmp_exp == nullptr || tmp_exp->class_type_ != CLASS_ARRAY) {
      std::cout << "Error: Can't Find Such Array" << std::endl;
      exit(0);
    }
    ArrayClass *current_array = (ArrayClass *) tmp_exp;

    tmp_exp = EvalExpression(tree->sons_[1], symbols);
    if (tmp_exp->class_type_ != CLASS_INT) {
      std::cout << "Error: Data Index Is Not Int" << std::endl;
      exit(0);
    }
    int idx = ((IntClass *) tmp_exp)->data_;
    if (idx < 0 || idx >= current_array->length_) {
      std::cout << "Error: Index Out of Range" << std::endl;
      exit(0);
    }
    tmp_exp = EvalExpression(tree->sons_[2], symbols);
    if (tmp_exp->class_type_ != CLASS_INT) {
      std::cout << "Error: Right Hand Value Is Not Int" << std::endl;
      exit(0); 
    }
    int r_value = ((IntClass *) tmp_exp)->data_;
    current_array->data_[idx] = r_value;
  }
  else {
    std::cout << "Error: This Is Not a Statement" << std::endl;
  }
}

BaseClass* Interpreter::EvalExpression(ParseTree *tree, SymbolTable &symbols) {
  if (tree->head_ == EXPRESSION_BIOP) {
    BaseClass *l_exp, *r_exp;
    l_exp = EvalExpression(tree->sons_[0], symbols);
    r_exp = EvalExpression(tree->sons_[2], symbols);
    std::string op = tree->sons_[1]->content_;
    if (op == "&&" || op == "||") {
      if (l_exp->class_type_ != CLASS_BOOL || r_exp->class_type_ != CLASS_BOOL) {
        std::cout << "Error: Invalid Expression Type" << std::endl;
        exit(0);
      }
      if (op == "&&")
        return
          (BaseClass *) new BoolClass(((BoolClass *) l_exp)->data_ && ((BoolClass *) r_exp)->data_);
      else
        return
          (BaseClass *) new BoolClass(((BoolClass *) l_exp)->data_ || ((BoolClass *) r_exp)->data_);
    }
    else if (op == "<" || op == ">") {
      if (l_exp->class_type_ != CLASS_INT || r_exp->class_type_ != CLASS_INT) {
        std::cout << "Error: Invalid Expression Type" << std::endl;
        exit(0);
      }
      if (op == "<")
        return
          (BaseClass *) new BoolClass(((IntClass *) l_exp)->data_ < ((IntClass *) r_exp)->data_);
      else
        return
          (BaseClass *) new BoolClass(((IntClass *) l_exp)->data_ > ((IntClass *) r_exp)->data_);
    }
    else if (op == "+" || op == "-" || op == "*" || op == "/") {
      if (l_exp->class_type_ != CLASS_INT || r_exp->class_type_ != CLASS_INT) {
        std::cout << "Error: Invalid Expression Type" << std::endl;
        exit(0);
      }
      if (op == "+")
        return
          (BaseClass *) new IntClass(((IntClass *) l_exp)->data_ + ((IntClass *) r_exp)->data_);
      else if (op == "-")
        return
          (BaseClass *) new IntClass(((IntClass *) l_exp)->data_ - ((IntClass *) r_exp)->data_);
      else if (op == "*")
        return
          (BaseClass *) new IntClass(((IntClass *) l_exp)->data_ * ((IntClass *) r_exp)->data_);
      else
        return
          (BaseClass *) new IntClass(((IntClass *) l_exp)->data_ / ((IntClass *) r_exp)->data_);
    }
    else {
      std::cout << "Error: Invalid Operation Type" << std::endl;
      exit(0);
    }
  }
  else if (tree->head_ == EXPRESSION_INDEX) {
    BaseClass *tmp_exp = EvalExpression(tree->sons_[0], symbols);
    if (tmp_exp->class_type_ != CLASS_ARRAY) {
      std::cout << "Error: It Is Not a Array" << std::endl;
      exit(0);
    }
    ArrayClass *current_array = (ArrayClass *) tmp_exp;
    tmp_exp = EvalExpression(tree->sons_[1], symbols);
    if (tmp_exp->class_type_ != CLASS_INT) {
      std::cout << "Error: Index Is Not Int" << std::endl;
      exit(0);
    }
    int idx = ((IntClass *) tmp_exp)->data_;
    if (idx < 0 || idx >= current_array->length_) {
      std::cout << "Error: Index out of Range" << std::endl;
      exit(0);
    }
    return (BaseClass *) new IntClass(current_array->data_[idx]);
  }
  else if (tree->head_ == EXPRESSION_LENGTH) {
    BaseClass *tmp_exp = EvalExpression(tree->sons_[0], symbols);
    if (tmp_exp->class_type_ != CLASS_ARRAY) {
      std::cout << "Error: It Is Not a Array" << std::endl;
      exit(0);
    }
    ArrayClass *current_array = (ArrayClass *) tmp_exp;
    return (BaseClass *) new IntClass(current_array->length_);
  }
  else if (tree->head_ == EXPRESSION_FUNCTION) {
    // { EXPRESSION, ID, ONE_OR_ZERO, PARAS }
    BaseClass *tmp_exp = EvalExpression(tree->sons_[0], symbols);
    if (tmp_exp->class_type_ != CLASS_CLASS) {
      std::cout << "Error: It Is Not a Class" << std::endl;
      exit(0);
    }
    ClassClass *current_class = (ClassClass *) tmp_exp;
    std::string id = tree->sons_[1]->content_;
    if ((current_class->functions_).find(id) == (current_class->functions_).end()) {
      std::cout << "Error: No Such Function" << std::endl;
    }
    Function *func = (current_class->functions_)[id];
    std::vector<BaseClass *> inputs;
    inputs.clear();
    if (tree->sons_.size() > 2) {
      auto paras_node = tree->sons_[2];
      for (auto son : paras_node->sons_) {
        inputs.push_back(EvalExpression(son, symbols));
      }
    }
    symbols.Add("this", (BaseClass *) current_class);
    return func->Execute(inputs, symbols);
    symbols.Del("this");
  }
  else if (tree->head_ == EXPRESSION_INT) {
    std::string str = tree->sons_[0]->content_;
    int val = 0;
    int begin_idx = str[0] == '-' ? 1 : 0; 
    for (int i = begin_idx; i < str.length(); i++) {
      val = val * 10 + (int) (str[i] - '0');
    }
    if (begin_idx == 1) {
      val = -val;
    }
    return (BaseClass *) new IntClass(val);
  }
  else if (tree->head_ == EXPRESSION_BOOL) {
    if (tree->sons_[0]->head_ == TRUE) {
      return (BaseClass *) new BoolClass(true);
    }
    else {
      return (BaseClass *) new BoolClass(false);
    }
  }
  else if (tree->head_ == EXPRESSION_ID || tree->head_ == EXPRESSION_THIS) {
    return symbols.Find(tree->sons_[0]->content_);
  }
  else if (tree->head_ == EXPRESSION_NEW_ARRAY) {
    auto tmp_ptr = EvalExpression(tree->sons_[2], symbols);
    if (tmp_ptr->class_type_ != CLASS_INT) {
      std::cout << "Error: Index is Not Int" << std::endl;
      exit(0);
    }
    return (BaseClass *) new ArrayClass(((IntClass *) tmp_ptr)->data_);
  }
  else if (tree->head_ == EXPRESSION_NEW_CLASS) {
    std::string id = tree->sons_[1]->content_;
    ClassClass* new_class = (ClassClass *) class_table_.DeepCopyFrom(id);
    if (new_class == nullptr) {
      std::cout << "Error: No Such Class" << std::endl;
      exit(0);
    }
    return (BaseClass *) new_class;
  }
  else if (tree->head_ == EXPRESSION_NT) {
    BaseClass *tmp_exp = EvalExpression(tree->sons_[0], symbols);
    if (tmp_exp->class_type_ != CLASS_BOOL) {
      std::cout << "Error: Invalid Data Type" << std::endl;
      exit(0);
    }
    BoolClass *new_bool = (BoolClass *) tmp_exp;
    new_bool->data_ = !new_bool->data_;
    return (BaseClass *) new_bool;
  }
  else if (tree->head_ == EXPRESSION_BRACKET) {
    return EvalExpression(tree->sons_[0], symbols);
  }
  else {
    std::cout << "Invalid Expresion" << std::endl;
    return 0;
  }
}

void Interpreter::PrintData(BaseClass *data) {
  if (data->class_type_ == CLASS_INT) {
    std::cout << ((IntClass *) data)->data_ << std::endl;
  }
  else if (data->class_type_ == CLASS_BOOL) {
    std::cout << ((BoolClass *) data)->data_ << std::endl;
  }
  else if (data->class_type_ == CLASS_ARRAY) {
    auto array_ptr = (ArrayClass *) data;
    for (int i = 0; i < array_ptr->length_; i++) {
      std::cout << array_ptr->data_[i] << " ";
    }
    std::cout << std::endl;
  }
  else {
    std::cout << "Error: Can't Print" << std::endl;
  }
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
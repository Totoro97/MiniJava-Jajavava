#include "Lexer.h"
#include <vector>
#include <string>

/* token rules:
ID: [a-zA-Z_]+
INT: -*[0-9]+
BOOL: {
  true, false
}
RESERVED: {
  class, public, static, void, main, String, extends, int, boolean, return,
  if, else, while, System.out.println
  this, new
}
SYMBOL: {
  , ; { } [ ] ( )...
}
OP: {
  + - * && ... < >
}
*/

ManualLexer::ManualLexer() {
  heads_.clear();
  NFANode *head;

  auto AddEdge = [](NFANode *a, NFANode *b, char l, char r){
    for (int i = int(l); i <= int(r); i++) {
      a->nex_[i] = b;
    }
  };

  // ID 
  head = new NFANode();
  auto tmp = new NFANode();
  AddEdge(head, tmp, 'a', 'z');
  AddEdge(head, tmp, 'A', 'Z');
  AddEdge(head, tmp, '_', '_');
  tmp->valid_ = true;
  heads_.push_back(head);

  // INT
  head = new NFANode();
  auto int_tmp_0 = new NFANode();
  auto int_tmp_1 = new NFANode();
  AddEdge(head, int_tmp_0, '-', '-');
  AddEdge(head, int_tmp_1, '0', '9');
  AddEdge(int_tmp_0, int_tmp_1, '0', '9');
  heads_.push_back(head);

  // RESERVED
  auto AddPath = [](NFANode *head, std::string path, TokenTag tag) {
    for (uint32_t i = 0; i < path.length(); i++) {
      if (!head->nex_[int(path[i])]) {
        auto tmp = new NFANode();
        head->nex_[int(path[i])] = tmp;
      }
      head = head->nex_[int(path[i])];
    }

    head->valid_ = true;
    head->tag_ = tag;
  };

  head = new NFANode();
  AddPath(head, std::string("true"), BOOL);
  AddPath(head, std::string("false"), BOOL);
  AddPath(head, std::string("class"), RESERVED);
  AddPath(head, std::string("public"), RESERVED);
  AddPath(head, std::string("static"), RESERVED);
  AddPath(head, std::string("void"), RESERVED);
  AddPath(head, std::string("main"), RESERVED);
  AddPath(head, std::string("String"), RESERVED);
  AddPath(head, std::string("extends"), RESERVED);
  AddPath(head, std::string("int"), RESERVED);
  AddPath(head, std::string("boolean"), RESERVED);
  AddPath(head, std::string("return"), RESERVED);
  AddPath(head, std::string("if"), RESERVED);
  AddPath(head, std::string("else"), RESERVED);
  AddPath(head, std::string("System.out.println"), RESERVED);
  AddPath(head, std::string("this"), RESERVED);
  AddPath(head, std::string("new"), RESERVED);
  AddPath(head, std::string(","), SYMBOL);
  AddPath(head, std::string(";"), SYMBOL);
  AddPath(head, std::string("{"), SYMBOL);
  AddPath(head, std::string("}"), SYMBOL);
  AddPath(head, std::string("["), SYMBOL);
  AddPath(head, std::string("]"), SYMBOL);
  AddPath(head, std::string(")"), SYMBOL);
  AddPath(head, std::string("("), SYMBOL);
  AddPath(head, std::string("+"), OP);
  AddPath(head, std::string("-"), OP);
  AddPath(head, std::string("&&"), OP);
  AddPath(head, std::string("*"), OP);
  
  heads_.push_back(head);
  return; 
}

std::string ManualLexer::GetTokens(std::ifstream &in_stream, std::vector<Token> &tokens) {
   
}
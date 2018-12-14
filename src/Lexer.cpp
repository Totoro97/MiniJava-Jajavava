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
  
}

std::string ManualLexer::GetTokens(std::ifstream &in_stream, std::vector<Token> &tokens) {
   
}
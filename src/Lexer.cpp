#include "Lexer.h"
#include <cstring>
#include <iostream>
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
  , ; { } [ ] ( ) . ! ...
}
OP: {
  + - * && ... < > =
}
*/

ManualLexer::ManualLexer() {
  heads_.clear();
  NFANode *head;

  auto AddEdge = [](NFANode *a, NFANode *b, char l, char r) {
    for (int i = int(l); i <= int(r); i++) {
      a->nex_[i] = b;
    }
  };

  auto AddWhiteSpaceEdge = [](NFANode *a, NFANode *b) {
    a->nex_[(int) '\n'] = a->nex_[(int) '\r'] = a->nex_[(int) '\t'] = a->nex_[(int) ' '] = b;
  };

  // ID
  head = new NFANode();
  auto id_tmp_0 = new NFANode();
  auto id_tmp_1 = new NFANode();
  AddEdge(head, id_tmp_0, 'a', 'z');
  AddEdge(head, id_tmp_0, 'A', 'Z');
  AddEdge(head, id_tmp_0, '_', '_');
  AddEdge(id_tmp_0, id_tmp_1, 'a', 'z');
  AddEdge(id_tmp_0, id_tmp_1, 'A', 'Z');
  AddEdge(id_tmp_0, id_tmp_1, '_', '_');
  AddEdge(id_tmp_0, id_tmp_1, '0', '9');
  AddEdge(id_tmp_1, id_tmp_1, 'a', 'z');
  AddEdge(id_tmp_1, id_tmp_1, 'A', 'Z');
  AddEdge(id_tmp_1, id_tmp_1, '_', '_');
  AddEdge(id_tmp_1, id_tmp_1, '0', '9');
  AddWhiteSpaceEdge(head, head);
  id_tmp_0->valid_ = true;
  id_tmp_0->tag_ = ID;
  id_tmp_1->valid_ = true;
  id_tmp_1->tag_ = ID;
  heads_.push_back(head);

  // INT
  head = new NFANode();
  auto int_tmp_0 = new NFANode();
  auto int_tmp_1 = new NFANode();
  AddEdge(head, int_tmp_0, '-', '-');
  AddEdge(head, int_tmp_1, '0', '9');
  AddEdge(int_tmp_0, int_tmp_1, '0', '9');
  AddEdge(int_tmp_1, int_tmp_1, '0', '9');
  AddWhiteSpaceEdge(head, head);
  int_tmp_1->valid_ = true;
  int_tmp_1->tag_ = INT;
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
  AddWhiteSpaceEdge(head, head);
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
  AddPath(head, std::string("."), SYMBOL);
  AddPath(head, std::string("+"), OP);
  AddPath(head, std::string("-"), OP);
  AddPath(head, std::string("&&"), OP);
  AddPath(head, std::string("*"), OP);
  AddPath(head, std::string("="), OP);
  AddPath(head, std::string("<"), OP);
  AddPath(head, std::string(">"), OP);
  AddPath(head, std::string("!"), OP);
  heads_.push_back(head);

  // COMMENT
  head = new NFANode();
  auto com_tmp_0 = new NFANode();
  auto com_tmp_1 = new NFANode();
  auto com_tmp_3 = new NFANode();
  AddWhiteSpaceEdge(head, head);
  AddEdge(head, com_tmp_0, '/', '/');
  AddEdge(com_tmp_0, com_tmp_1, '/', '/');
  AddEdge(com_tmp_1, com_tmp_1, (char) 0, (char) 127);
  AddEdge(com_tmp_1, com_tmp_3, '\n', '\n');
  AddEdge(com_tmp_1, com_tmp_3, '\r', '\r');
  AddEdge(com_tmp_3, com_tmp_3, '\n', '\n');
  AddEdge(com_tmp_3, com_tmp_3, '\r', '\r');
  com_tmp_3->valid_ = true;
  com_tmp_3->tag_ = COMMENT;
  heads_.push_back(head);

  head = new NFANode();
  com_tmp_0 = new NFANode();
  com_tmp_1 = new NFANode();
  auto com_tmp_2 = new NFANode();
  com_tmp_3 = new NFANode();

  AddWhiteSpaceEdge(head, head);
  AddEdge(head, com_tmp_0, '/', '/');
  AddEdge(com_tmp_0, com_tmp_1, '*', '*');
  AddEdge(com_tmp_1, com_tmp_1, (char) 0, (char) 127);
  AddEdge(com_tmp_1, com_tmp_2, '*', '*');
  AddEdge(com_tmp_2, com_tmp_1, (char) 0, (char) 127);
  AddEdge(com_tmp_2, com_tmp_2, '*', '*');
  AddEdge(com_tmp_2, com_tmp_3, '/', '/');
  com_tmp_3->valid_ = true;
  com_tmp_3->tag_ = COMMENT;
  heads_.push_back(head);

  std::cout << "yes" << std::endl;
}

std::string ManualLexer::GetTokens(std::ifstream &in_stream, std::vector<Token> &tokens) {
  char *s = new char[10240];
  std::memset(s, 0, 10240);
  int n = 0;
  do s[n++] = (char) in_stream.get(); while (s[n - 1] != -1);
  s[n - 1] = '\n';
  std::cout << "n = " << n << std::endl;
  std::vector<NFANode *> nodes = heads_;
  int res = heads_.size();
  int past = 0;
  int last_valid = -1;
  TokenTag ans_tag;
  for (int i = 0; i < n; ) {
    int v = static_cast<int>(s[i]);
    for (int j = 0; j < nodes.size(); j++) {
      if (nodes[j] == nullptr)
        continue;
      nodes[j] = nodes[j]->nex_[v];
      if (nodes[j] == nullptr) {
        res--;
      } else if (nodes[j]->valid_) {
        ans_tag = nodes[j]->tag_;
        last_valid = i;
      }
    }
    if (res == 0) {
      if (past == last_valid + 1) {
        return std::string("Error");
      }
      while(s[past] == '\n' || s[past] == '\r' || s[past] == ' ' || s[past] == '\t')
        past++;
      // std::cout << "past = " << past << " last_valid = " << last_valid << std::endl;
      tokens.emplace_back(ans_tag, std::string(s + past, last_valid - past + 1));
      // std::cout << tokens.back().chars << std::endl;
      past = i = last_valid + 1;
      for (int j = 0; j < heads_.size(); j++) {
        nodes[j] = heads_[j];
      }
      res = heads_.size();
      if (i == n - 1) {
        break;
      }
    } else {
      i++;
    }
  }
  return std::string("OK");
}
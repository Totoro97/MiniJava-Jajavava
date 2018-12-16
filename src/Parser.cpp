#include "Parser.h"

#include <list>
#include <set>
#include <stack>
#include <vector>

ManualParser::ManualParser() {

  AddRule(GOAL, { MAIN_CLASS, MANY_OR_ZERO, CLASS_DECLARATION });
  AddRule(MAIN_CLASS,
          { CLASS, ID, LCUR, PUBLIC, VOID, MAIN, LB, STRING, LSQR, RSQR, ID, RB, LCUR, STATEMENT, RCUR, RCUR});
  AddRule(CLASS_DECLARATION,
          { CLASS, ID, ONE_OR_ZERO, EXTENDS_IDENTIFIER, LCUR, MANY_OR_ZERO, VAR_DECLARATION,
            MANY_OR_ZERO, METHOD_DECLARATION, RCUR});
  AddRule(VAR_DECLARATION, { TYPE, ID, SEMI });
  AddRule(METHOD_DECLARATION,
          { PUBLIC, TYPE, ID, LB, ONE_OR_ZERO, TYPE_ID_MANY, RB, LCUR, MANY_OR_ZERO, VAR_DECLARATION,
            MANY_OR_ZERO, STATEMENT, RETURN, EXPRESSION, SEMI, RCUR});
  AddRule(TYPE_ID_MANY, { TYPE, ID, MANY_OR_ZERO, DOT_TYPE_ID });
  AddRule(DOT_TYPE_ID, { DOT, TYPE, ID });
  AddRule(TYPE, { INT, LSQR, RSQR });
  AddRule(TYPE, { BOOLEAN });
  AddRule(TYPE, { INT });
  AddRule(TYPE, { ID });
  AddRule(STATEMENT, { LCUR, ONE_OR_ZERO, STATEMENT, RCUR });
  AddRule(STATEMENT, { IF, LB, EXPRESSION, RB, STATEMENT, ELSE, STATEMENT});
  AddRule(STATEMENT, { WHILE, LB, EXPRESSION, RB, STATEMENT });
  AddRule(STATEMENT, { PRINT, LB, EXPRESSION, RB, SEMI});
  AddRule(STATEMENT, { ID, EQ, EXPRESSION, SEMI });
  AddRule(STATEMENT, { ID, LSQR, EXPRESSION, RSQR, EQ, EXPRESSION, SEMI });
  AddRule(EXPRESSION, { EXPRESSION, BIOP, EXPRESSION });
  AddRule(EXPRESSION, { EXPRESSION, LSQR, EXPRESSION, RSQR });
  AddRule(EXPRESSION, { EXPRESSION, DOT, LENGTH });
  AddRule(EXPRESSION, { EXPRESSION, DOT, ID, LB, ONE_OR_ZERO, PARAS, RB});
  AddRule(PARAS, { EXPRESSION, MANY_OR_ZERO, DOT_EXPRESSION });
  AddRule(DOT_EXPRESSION, { DOT, EXPRESSION });
  AddRule(EXPRESSION, { INT_LITERAL });
  AddRule(EXPRESSION, { TRUE });
  AddRule(EXPRESSION, { FALSE });
  AddRule(EXPRESSION, { ID });
  AddRule(EXPRESSION, { THIS });
  AddRule(EXPRESSION, { NEW, INT, LSQR, EXPRESSION, RSQR });
  AddRule(EXPRESSION, { NEW, ID, LB, RB });
  AddRule(EXPRESSION, { NT, EXPRESSION });
  AddRule(EXPRESSION, { LB, EXPRESSION, RB });
}

void ManualParser::AddRule(TokenTag head, std::vector<TokenTag> form) {
  auto init_node = new NFANode();
  std::vector<NFANode *> current_nodes = { init_node };
  for (auto iter = form.begin(); iter != form.end(); iter++) {
    auto judge_token = *iter;
    auto token = *iter;
    if (judge_token == MANY_OR_ZERO || ONE_OR_ZERO) {
      token = *(++iter);
    }
    auto new_node = new NFANode();
    for (auto current_node : current_nodes) {
      current_node->nex_[token] = new_node;
    }
    if (judge_token == MANY_OR_ZERO) {
      new_node->nex_[token] = new_node;
    }
    else if (token != ONE_OR_ZERO) {
      current_nodes.clear();
    };
    current_nodes.push_back(new_node);
  }
  for (auto node : current_nodes) {
    node->valid_ = true;
    node->tag_ = head;
  }
  
  rules_.emplace_back(head, init_node);
}

ManualParser::~ManualParser() {}

std::string ManualParser::GetParseTree(const std::vector<Token> &tokens, ParseTree* &parse_tree) {
  std::vector<std::list<Rule> > paths;
  paths.emplace_back();
  for (const auto &rule : rules_) {
    paths.back().push_back(rule);
  }
  int n = tokens.size();
  std::vector<ParseTree *> st = { new ParseTree(DEFAULT, {}, "") };
  for (int i = n - 1; i >= 0; i--) {
    Token token = tokens[i];
    if (token.tag == COMMENT) {
      continue;
    }
    auto node = new ParseTree(token.tag, {}, token.chars);
    st.push_back(node);
  }
  
  int now_pt = st.size() - 1;
  int ok_pt = st.size();
  TokenTag ok_tag = DEFAULT;
  bool last_failure = false;
  while (st.size() > 2) {
    ParseTree *node = st[now_pt];
    std::list<Rule> rules = paths.back();
    for (auto iter = rules.begin(); iter != rules.end(); ) {
      auto &rule = *iter;
      rule.head_ = rule.head_->nex_[node->head_];
      if (rule.head_ == nullptr) {
        iter = rules.erase(iter);
      } else {
        if (rule.head_->valid_) {
          ok_pt = now_pt;
          ok_tag = rule.result_;
        }
        iter++;
      }
    }
    if (rules.empty()) {
      if (ok_pt != st.size()) {
        last_failure = false;
        auto new_node = new ParseTree(ok_tag, {}, "");
        while (ok_pt != st.size()) {
          new_node->sons_.push_back(st.back());
          st.pop_back();
        }
        st.push_back(new_node);
        paths.pop_back();
        now_pt = st.size() - 1;
      } else {
        if (last_failure) {
          return "Error!";
        }
        paths.emplace_back();
        for (const auto &rule : rules_) {
          paths.back().push_back(rule);
        }
        last_failure = true;
      }
    }
    else {
      now_pt--;
      last_failure = false;
    }
  }
  parse_tree = st.back();
  return "ok";
}
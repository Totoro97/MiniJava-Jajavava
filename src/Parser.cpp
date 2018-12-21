#include "Parser.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <set>
#include <stack>
#include <vector>

ManualParser::ManualParser() {
  std::cout << "Construct Parser: Begin" << std::endl;
  AddRule(GOAL, { MAIN_CLASS, MANY_OR_ZERO, CLASS_DECLARATION });
  AddRule(MAIN_CLASS,
          { CLASS, ID, LCUR, PUBLIC, STATIC, VOID, MAIN, LB, STRING, LSQR, RSQR, ID, RB, LCUR, STATEMENT, RCUR, RCUR});
  AddRule(CLASS_DECLARATION,
          { CLASS, ID, ONE_OR_ZERO, EXTENDS_IDENTIFIER, LCUR, MANY_OR_ZERO, VAR_DECLARATION,
            MANY_OR_ZERO, METHOD_DECLARATION, RCUR});
  AddRule(VAR_DECLARATION, { TYPE, ID, SEMI });
  AddRule(METHOD_DECLARATION,
          { PUBLIC, TYPE, ID, LB, ONE_OR_ZERO, TYPE_ID_MANY, RB, LCUR, MANY_OR_ZERO, VAR_DECLARATION,
            MANY_OR_ZERO, STATEMENT, RETURN, EXPRESSION, SEMI, RCUR});
  AddRule(TYPE_ID_MANY, { TYPE, ID, MANY_OR_ZERO, DOT_TYPE_ID });
  AddRule(DOT_TYPE_ID, { COMMA, TYPE, ID });
  AddRule(TYPE, { INT, LSQR, RSQR });
  AddRule(TYPE, { BOOLEAN });
  AddRule(TYPE, { INT });
  AddRule(TYPE, { ID });
  AddRule(STATEMENT, { LCUR, MANY_OR_ZERO, STATEMENT, RCUR });
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
  AddRule(DOT_EXPRESSION, { COMMA, EXPRESSION });
  AddRule(EXPRESSION, { INT_LITERAL });
  AddRule(EXPRESSION, { TRUE });
  AddRule(EXPRESSION, { FALSE });
  AddRule(EXPRESSION, { ID });
  AddRule(EXPRESSION, { THIS });
  AddRule(EXPRESSION, { NEW, INT, LSQR, EXPRESSION, RSQR });
  AddRule(EXPRESSION, { NEW, ID, LB, RB });
  AddRule(EXPRESSION, { NT, EXPRESSION });
  AddRule(EXPRESSION, { LB, EXPRESSION, RB });

  std::cout << "Add Rule: Done" << std::endl;

  std::cout << "Construct Parser: Done" << std::endl;
}

void ManualParser::AddRule(TokenTag head, std::vector<TokenTag> form) {
  auto init_node = new NFANode();
  std::vector<NFANode *> current_nodes = { init_node };
  for (auto iter = form.begin(); iter != form.end(); iter++) {
    auto judge_token = *iter;
    auto token = *iter;
    if (judge_token == MANY_OR_ZERO || judge_token == ONE_OR_ZERO) {
      token = *(++iter);
    }
    auto new_node = new NFANode();
    for (auto current_node : current_nodes) {
      current_node->nex_[token] = new_node;
    }
    if (judge_token == MANY_OR_ZERO) {
      new_node->nex_[token] = new_node;
    }
    else if (judge_token != ONE_OR_ZERO) {
      current_nodes.clear();
    }
    current_nodes.push_back(new_node);
  }
  for (auto node : current_nodes) {
    node->valid_ = true;
    node->tag_ = head;
  }
  
  rules_.emplace_back(head, init_node);
  NFAs_[head].push_back(init_node);

  for (int i = 0; i < TokenTag::END; i++) {
    if (NFAs_[i].empty()) {
      termis_[i].insert(TokenTag(i));
    }
  }
  while (true) {
    bool no_change = true;
    for (int i = 0; i < TokenTag::END; i++) {
      auto current_size = termis_[i].size();
      for (auto node : NFAs_[i]) {
        for (int j = 0; j < TokenTag::END; j++) {
          if (node->nex_[j]) {
            for (auto token : termis_[j]) {
              termis_[i].insert(token);
            }
          }
        }
      }
      if (current_size != termis_[i].size()) {
        no_change = false;
      }
    }
    if (no_change) {
      break;
    }
  }
}

ManualParser::~ManualParser() = default;

void ManualParser::Enclosure(std::set<State> &wait_pool) {
  while (true) {
    std::set<State> tmp;
    tmp.clear();
    for (const auto &state : wait_pool) {
      for (int i = TokenTag::DEFAULT; i < TokenTag::END; i++) {
        if (state.node->nex_[i] != nullptr) {
          auto tmp_node = state.node->nex_[i];
          for (int j = TokenTag::DEFAULT; j < TokenTag::END; j++) {
            if (tmp_node->nex_[j] != nullptr) {
              for (auto new_node : NFAs_[i]) {
                for (auto termi : termis_[j]) {
                  tmp.emplace(state.r, state.r, new_node, TokenTag(i), TokenTag(termi), new ParseTree(TokenTag(i), {}, ""));
                }
              }
            }
          }
          if (tmp_node->valid_) {
            for (auto new_node : NFAs_[i]) {
              tmp.emplace(state.r, state.r, new_node, TokenTag(i), state.follow, new ParseTree(TokenTag(i), {}, ""));
            }
          }
        }
      }
    }
    auto past_size = wait_pool.size();
    for (const auto &new_state : tmp) {
      wait_pool.insert(new_state);
    }
    if (past_size == wait_pool.size()) {
      break;
    }
  }
}

void ManualParser::PrintContent(ParseTree *parse_tree) {
  std::cout << parse_tree->content_;
  for (auto son : parse_tree->sons_) {
    PrintContent(son);
  }
}

std::string ManualParser::GetParseTree(const std::vector<Token> &tokens, ParseTree* &parse_tree) {
  std::set<State> wait_pool_0, wait_pool_1;
  wait_pool_0.clear();
  for (auto node : NFAs_[GOAL]) {
    wait_pool_0.insert(State(0, 0, node, GOAL, DEFAULT, new ParseTree(GOAL, {}, "")));
  }
  /*for (auto node : NFAs_[STATEMENT]) {
    wait_pool_0.insert(State(0, 0, node, STATEMENT, DEFAULT, new ParseTree(STATEMENT, {}, "")));
  }*/
  std::vector<std::pair<ParseTree *, int> > st;
  st.clear();
  st.emplace_back(new ParseTree(DEFAULT, {}, ""), static_cast<int>(tokens.size()));
  for (int i = static_cast<int>(tokens.size()) - 1; i >= 0; i--) {
    const auto &token = tokens[i];
    if (token.tag == COMMENT) {
      continue;
    }
    st.emplace_back(new ParseTree(token.tag, {}, token.chars), i);
  }
  auto wait_pool = &wait_pool_0;
  auto new_pool = &wait_pool_1;
  while (st.size() > 0) {
    int now_pt = st.size() - 1;
    auto now_parse_tree = st[now_pt].first;
    auto token = now_parse_tree->head_;
    int current_pos = st[now_pt].second;
    auto next_token = now_pt > 0 ? st[now_pt - 1].first->head_ : DEFAULT;
    int next_pos = now_pt > 0 ? st[now_pt - 1].second : current_pos;
    Enclosure(*wait_pool);
    std::cout << "wait_pool size = " << wait_pool->size() << std::endl;
    std::cout << "Token = " << token << " " << now_parse_tree->content_ << std::endl;
    new_pool->clear();
    if (current_pos >= next_pos && st.size() > 1) {
      std::cout << "fuck" << std::endl;
      return "";
    }
    int num_reduce = 0;
    for (const auto &state : *wait_pool) {
      if (state.node->valid_ && state.follow == token && state.r == current_pos) {
        std::cout << "eat it! follow = " << state.follow << std::endl;
        PrintContent(state.parse_tree);
        std::cout << std::endl;
        if (token == DEFAULT) {
          std::cout << "yes" << std::endl;
          parse_tree = state.parse_tree;
          return "OK";
        }
        num_reduce++;
        if (state.l >= state.r) {
          std::cout << "fuck" << std::endl;
        }
        st.emplace_back(state.parse_tree, state.l);
      } else {
        new_pool->insert(state);
      }
    }
    if (num_reduce > 1) {
      std::cout << "Error!!!!!!" << std::endl;
      return "Error";
    }
    if (num_reduce == 0) {
      bool can_go_on = false;
      auto LookForward = [this](NFANode *node, TokenTag token, TokenTag follow) {
        for (int i = 0; i < TokenTag::END; i++) {
          if (node->nex_[i] == nullptr) {
            continue;
          }
          if (termis_[i].find(token) != termis_[i].end()) {
            return true;
          }
        }
        return (node->valid_ && token == follow);
      };

      for (const auto &state : *wait_pool) {
        if (state.r == current_pos && state.node->nex_[token] != nullptr &&
          LookForward(state.node->nex_[token], next_token, state.follow)) {
          new_pool->emplace(state.l, next_pos, state.node->nex_[token], state.result, state.follow, state.parse_tree);
          state.parse_tree->sons_.push_back(now_parse_tree);
          can_go_on = true;
        }
      }
      if (!can_go_on) {
        return "Error";
      }
      std::cout <<"pop back" << std::endl;
      st.pop_back();
    }
    std::swap(new_pool, wait_pool);
  }
  return "OK";
}

/*std::string ManualParser::GetParseTree(const std::vector<Token> &tokens, ParseTree* &parse_tree) {
  std::vector<std::list<Rule> > paths;
  paths.emplace_back();
  for (const auto &rule : rules_) {
    paths.back().push_back(rule);
  }
  int n = tokens.size();
  std::cout << "n = " << n << std::endl;
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
    std::cout << "st size = " << st.size() << " now token = " << st[now_pt]->head_ << std::endl;
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
      paths.pop_back();
      paths.push_back(rules);
      now_pt--;
      last_failure = false;
    }
  }
  parse_tree = st.back();
  return "ok";
}*/
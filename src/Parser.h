#include "Lexer.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class Rule {
public:
  TokenTag result_;
  NFANode *head_;
  std::vector<TokenTag> filter_;
  TokenTag abstract_tag_;
  int rule_id_;

  Rule(TokenTag result = TokenTag::DEFAULT,
          NFANode *head = nullptr,
          std::vector<TokenTag> filter = {},
          TokenTag abstract_tag = TokenTag::DEFAULT,
          int rule_id = -1) :
          result_(result), head_(head), filter_(filter), abstract_tag_(abstract_tag), rule_id_(rule_id) {}

  bool operator < (const Rule &b) const {
    return head_ < b.head_;
  }
};

class ParseTree {
public:
  TokenTag head_;
  Rule *rule_;
  std::vector<ParseTree *> sons_;
  std::string content_, comment_;

  ParseTree(TokenTag head = DEFAULT, Rule *rule = nullptr,
          std::vector<ParseTree *> sons = {}, std::string content = "") {
    head_ = head;
    rule_ = rule;
    sons_ = sons;
    content_ = content;
  }
};

class Parser {
public:
  Parser() {}
  ~Parser() {}
  virtual std::string GetParseTree(const std::vector<Token> &tokens, ParseTree* &parse_tree) {
    return "";
  }
};

struct State {
  int l, r;
  NFANode *node;
  TokenTag result, follow;
  ParseTree *parse_tree;
  State(int l = 0, int r = 0, NFANode *node = nullptr,
          TokenTag result = DEFAULT, TokenTag follow = DEFAULT, ParseTree *parse_tree = nullptr) :
          l(l), r(r), node(node), result(result), follow(follow), parse_tree(parse_tree) {}
  bool operator < (const State &b) const {
    if (l != b.l) return l < b.l;
    if (r != b.r) return r < b.r;
    if (node != b.node) return node < b.node;
    if (result != b.result) return result < b.result;
    return follow < b.follow;
  }
};

class ManualParser : public Parser {
public:
  ManualParser();
  ~ManualParser();
  std::string GetParseTree(const std::vector<Token> &tokens, ParseTree* &parse_tree) final;
  void AddRule(TokenTag head, std::vector<TokenTag> form,
          std::vector<TokenTag> filter = {}, TokenTag abstract_tag = DEFAULT);
  std::vector<Rule> rules_[TokenTag::END];
  int rule_num_;
  std::vector<NFANode *> NFAs_[TokenTag::END];
  std::set<TokenTag> termis_[TokenTag::END];
  void Enclosure(std::set<State> &wait_pool);
  void PrintContent(ParseTree *parse_tree);
  ParseTree* FilterParseTree(ParseTree* node);
};

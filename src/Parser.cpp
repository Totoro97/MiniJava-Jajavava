#include "Parser.h"

ManualParser::ManualParser() {
  auto AddRule = [this](TokenTag head, std::vector<TokenTag> form) {
    auto tmp = new Rule(head, form);
    rules_.push_back(tmp);
  };

  AddRule(GOAL, { MAIN_CLASS, CLASS_DECLARATIONS });
  AddRule(GOAL, { MAIN_CLASS });
  AddRule(CLASS_DECLARATIONS, { CLASS_DECLARATION });
  AddRule(CLASS_DECLARATIONS, { CLASS_DECLARATIONS, CLASS_DECLARATION });
  AddRule(MAIN_CLASS,
          { CLASS, ID, LCUR, PUBLIC, VOID, MAIN, LB, STRING, LSQR, RSQR, ID, RB, LCUR, STATEMENT, RCUR, RCUR});
  AddRule(CLASS_DECLARATION,
          { CLASS, ID, EXTENDS_IDENTIFIER, LCUR, VAR_DECLARATIONS, METHOD_DECLARATIONS, RCUR});
  AddRule(CLASS_DECLARATION,
          { CLASS, ID, EXTENDS_IDENTIFIER, LCUR, METHOD_DECLARATIONS, RCUR});
  AddRule(CLASS_DECLARATION,
          { CLASS, ID, EXTENDS_IDENTIFIER, LCUR, VAR_DECLARATIONS, RCUR});
  AddRule(CLASS_DECLARATION,
          { CLASS, ID, EXTENDS_IDENTIFIER, LCUR, RCUR});
  AddRule(CLASS_DECLARATION,
          { CLASS, ID, LCUR, VAR_DECLARATIONS, METHOD_DECLARATIONS, RCUR});
  AddRule(CLASS_DECLARATION,
          { CLASS, ID, LCUR, METHOD_DECLARATIONS, RCUR});
  AddRule(CLASS_DECLARATION,
          { CLASS, ID, LCUR, VAR_DECLARATIONS, RCUR});
  AddRule(CLASS_DECLARATION,
          { CLASS, ID, LCUR, RCUR });
  AddRule(VAR_DECLARATIONS, { VAR_DECLARATION };
  AddRule(VAR_DECLARATIONS, { VAR_DECLARATIONS, VAR_DECLARATION });
  AddRule(VAR_DECLARATION, { TYPE, ID, SEMI });
  AddRule()

}

ManualParser::~ManualParser() {}

std::string ManualParser::GetParseTree(const std::vector<Token> &tokens, ParseTree &parse_tree) {
  return "";
}
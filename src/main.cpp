#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "Lexer.h"
#include "Parser.h"

void DrawParseTree(ParseTree *root, std::string sp) {
  std::cout << sp << token2str[root->head_] << " " << root->content_ << std::endl;
  for (auto son : root->sons_) {
    DrawParseTree(son, sp + " ");
  }
};

int main() {
  std::ifstream in_stream;
  in_stream.open("test.java", std::ios::in);
  std::vector <Token> tokens;
  auto lexer = new ManualLexer();
  auto err_info = lexer->GetTokens(in_stream, tokens);
  delete(lexer);
  in_stream.close();
  /*for (auto &token : tokens) {
    std::cout << token.tag << " " << token.chars << std::endl;
  }*/
  if (err_info != std::string("OK")) {
    std::cout << err_info << std::endl;
    return 0;
  }

  ParseTree* parse_tree;
  auto parser = new ManualParser();
  err_info = parser->GetParseTree(tokens, parse_tree);
  delete(parser);
  if (err_info != std::string("OK")) {
    std::cout << err_info << std::endl;
    return 0;
  } else {
    DrawParseTree(parse_tree, "");
  }
  return 0;
}
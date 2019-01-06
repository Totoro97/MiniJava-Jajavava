#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

void DrawParseTree(ParseTree *root, std::string sp) {
  std::cout << sp << token2str[root->head_] << " " << root->content_ << std::endl;
  for (auto son : root->sons_) {
    DrawParseTree(son, sp + "|  ");
  }
};

int main() {
  std::ifstream in_stream;
  in_stream.open("test.java", std::ios::in);
  std::vector <Token> tokens;

  std::cout << std::endl << "---------------------------- Lexer -----------------------------------"
            << std::endl << std::endl;
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
  std::cout << std::endl << "---------------------------- Parser ----------------------------------"
            << std::endl << std::endl;
  ParseTree *parse_tree = nullptr, *abstract_parse_tree = nullptr;
  auto parser = new ManualParser();
  err_info = parser->GetParseTree(tokens, parse_tree);
  abstract_parse_tree = parser->FilterParseTree(parse_tree);

  if (err_info != std::string("OK")) {
    std::cout << err_info << std::endl;
    return 0;
  } else {
    DrawParseTree(abstract_parse_tree, "");
    // DrawParseTree(parse_tree, "");
  }

  std::cout << std::endl << "----------------------- Aemantic Analysis ----------------------------"
    << std::endl << std::endl;

  // err_info = parser->Analysis(parse_tree);
  if (err_info != std::string("OK")){
  	std::cout << err_info << std::endl;
	  return 0;
  } else {
    std::cout << "yes" << std::endl;
  }

  std::cout << std::endl << "------------------------- Executed Result ----------------------------"
    << std::endl << std::endl;

  global_interpreter.Interprete(abstract_parse_tree);
  
  return 0;
}

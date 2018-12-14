#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

int main() {
  std::ifstream in_stream;
  in_stream.open("test.java", std::ios::in);
  
  std::vector <Token> tokens;
  lexer = new Lexer();
  auto err_info = lexer->GetTokens(in_stream, tokens);
  delete(lexer);
  in_stream.close();
  if (err_info != std::string("")) {
    std::cout << err_info << std::endl;
    return 0;
  }

  ParseTree parse_tree;
  parser = new Parser();
  err_info = paser->GetParseTree(tokens, parse_tree);
  delete(parser);
  if (err_info != std::string("")) {
    std::cout << err_info << std::endl;
    return 0;
  }
  return 0;
}
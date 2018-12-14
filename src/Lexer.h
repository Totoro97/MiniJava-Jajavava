#pragma once
#include <string>
#include <fstream>
#include <vector>

struct Token {
  
}

class Lexer {
  Lexer();
  std::string GetTokens(std::ifstream &in_stream, std::vector<Token> &tokens);  
};
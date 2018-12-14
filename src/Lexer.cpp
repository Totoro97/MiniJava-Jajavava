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
  
}

std::string ManualLexer::GetTokens(std::ifstream &in_stream, std::vector<Token> &tokens) {
  
}
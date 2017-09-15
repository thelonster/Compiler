#include "compiler.h"
#include <iostream>
#include <fstream>
#include <assert.h>

int main() {
	std::ifstream input;
	input.open("C:\\Users\\Lonnie\\Source\\Repos\\Compiler\\Compiler\\test.txt");
	std::string token;
	std::string lexeme;
    std::cout << "Token\tLexeme\n" << lexeme << "\t" << token << "\n";
    lexer(input);
	//token = gettoken(input);
	//lexeme = identifylexeme(token);
	input.close();
	return 0;
}
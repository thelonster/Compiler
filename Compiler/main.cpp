#include "compiler.h"
#include <iostream>
#include <fstream>
#include <assert.h>

int main() {
	std::ifstream input;
	input.open("C:\\Users\\Lonnie\\Source\\Repos\\Compiler\\Compiler\\test.txt");
	std::string token;
	std::string lexeme;
	token = gettoken(input);
	lexeme = identifylexeme(token);
	std::cout << "Token\tLexeme\n" << lexeme << "\t" << token << "\n";

	return 0;
}
#include "compiler.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>

int main() {
	std::ifstream input;
	input.open("C:\\Users\\Lonnie\\Source\\Repos\\Compiler\\Compiler\\test.txt");
	std::string token;
	std::string lexeme;
    std::cout << "Token\tLexeme" << std::endl;
    lexer(input);
	input.close();
	return 0;
}
#include "compiler.h"

std::vector<std::string> keywords = { "while", "if", "int", "fi", "else", "return", "read", "write" };
std::vector<char> separators = { '(', ')', '{', '}', '%', '@' };
std::vector<char> operators = { '+', '-', '/', '*', '<', '>', '=', ':', '!' };

void lexer(std::ifstream& input) {

}

std::string gettoken(std::ifstream& input) {
	std::string token;
	char c;
	while (input.get(c)) {
		if (isseparator(c)) {
			token += c;
			if (c == '%') {
				//add another if statement later to check if c is % again
				//incase you need to unget() (do this when making error handler)
				input.get(c);
				token += c;
			}
			return token;
		}
		else if (isoperator(c)) {
			token += c;
			input.get(c);
			if (c == '=')
				token += c;
			else
				input.unget();
			return token;
		}
		else if (isalnum(c))
			token += c;
		else
			break;
			
	}
	return token;
}

std::string identifylexeme(std::string lexeme) {
	char first = lexeme[0];
	if (isseparator(first))
		return "separator";
	else if (isoperator(first))
		return "operator";
	for (int a = 0; a < keywords.size(); a++)
		if (lexeme.compare(keywords[a]) == 0)
			return "keyword";
	return "unknown";
}

bool isseparator(char c) {
	for (auto it = separators.begin(); it != separators.end(); it++)
		if (*it == c)
			return true;
	return false;
}

bool isoperator(char c) {
	for (auto it = operators.begin(); it != operators.end(); it++)
		if (*it == c)
			return true;
	return false;
}
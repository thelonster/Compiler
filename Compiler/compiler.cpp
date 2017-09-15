#include "compiler.h"

std::vector<std::string> keywords = { "while", "if", "int", "fi", "else", "return", "read", "write" };
std::vector<char> separators = { '(', ')', '{', '}', '%', '@' };
std::vector<char> operators = { '+', '-', '/', '*', '<', '>', '=', ':', '!' };
std::vector<char> letters = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
std::vector<char> numbers = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

void lexer(std::ifstream& input) {
	std::string token;
	char c;
	bool floatflag = false, tokenfound = false, hashflag = false;
	while (input.get(c)) {
		tokenfound = false;
		switch (getcase(c)) {
			//This case is for ID's
			case 1:
				//state 1 -> 2: adding letter then moving to second state
				token += c;
				
				break;
			//This case is for ints and floats
			case 2:
				do {
					switch (numcase(c))	{
						//number case
						case 1:
							token += c;
							break;
						//period case
						case 2:
							floatflag = true;
							token += c;
							break;
						//terminating case
						default:
							input.unget();
							if (floatflag) {
								std::cout << token << " float" << std::endl;
							}
							else
								std::cout << token << " int" << std::endl;
							floatflag = false;
							token = "";
							tokenfound = true;
							break;
					}
				} while (input.get(c) && !tokenfound);				
				break;
		}
	}
}

int getcase(char c) {
	if (isalpha(c))
		return 1;
	else if (isdigit(c))
		return 2;
	else
		return -1;
}

int numcase(char c) {
	if (isdigit(c))
		return 1;
	else if (c == '.')
		return 2;
	else
		return -1;
}

int idcase(char c) {
	if (isalpha(c))
		return 1;
	else if (c == '#')
		return 2;
	else if (isdigit(c))
		return 3;
	else
		return -1;
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
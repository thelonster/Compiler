#include "compiler.h"

std::string gettoken(std::ifstream input) {
	std::string token;
	char c;
	while (input.get(c)) {
		if (isalnum(c))
			token += c;
		else
			break;
	}
	return token;
}

std::string getlexeme() {

}

bool isseparator(char c) {
	return 0;
}

bool isoperator(char c) {
	return 0;
}
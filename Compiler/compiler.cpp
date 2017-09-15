#include "compiler.h"

std::vector<std::string> keywords = { "while", "if", "int", "fi", "else", "return", "read", "write" };
std::vector<char> separators = { '(', ')', '{', '}', '%', '@' };
std::vector<char> operators = { '+', '-', '/', '*', '<', '>', '=', ':', '!' };
std::vector<char> letters = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
std::vector<char> numbers = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

void lexer(std::ifstream& input) {
	std::string token;
	char c;
	bool floatflag = false, tokenfound = false, hashflag = false, keyflag = false;
	while (input.get(c)) {
		tokenfound = false;
		switch (getstate(c)) {
			//This case is for ID's
			case 1:
				//state 1 -> 2: adding letter then moving to second state
				token += c;
				while (input.get(c) && !tokenfound) {
					switch (idstate(c)) {
                        //letter state
						case 1:
						    token += c;
                            hashflag = false; //if a number is added, reset hashflag
						    //break moves from state 3/6 to state 2
						    break;
                        //# state
                        case 2:
                            //Can't have 2 # signs in a row
                            if (hashflag) {
                                std::cout << token << " unknown" << std::endl;
                                tokenfound = true;
                                break;
                            }
                            token += c;
                            hashflag = true;
                            break;
                        //number state
                        case 3:
                            token += c;
                            hashflag = false;
                            break;
                        //accepting state
                        default:
                            input.unget();
                            tokenfound = true;
                            for (auto it = keywords.begin(); it != keywords.end(); it++) {
                                if (token == *it) {
                                    std::cout << token << " keyword" << std::endl;
                                    keyflag = true;
                                }
                            }
                            if (!keyflag)
                                std::cout << token << " identifier" << std::endl;
                            keyflag = false;
                            token = "";
                            break;
					}
				}
				break;
			//This state is for ints and floats
			case 2:
				do {
					switch (numstate(c))	{
						//number state
						case 1:
							token += c;
							break;
						//period state
						case 2:
							floatflag = true;
							token += c;
							break;
						//accepting state
						default:
							input.unget();
							if (floatflag)
								std::cout << token << " float" << std::endl;
							else
								std::cout << token << " int" << std::endl;
							floatflag = false;
							token = "";
							tokenfound = true;
							break;
					}
				} while (input.get(c) && !tokenfound);				
				break;
            //state for operators & separators (also blank spaces)
            default:
                if (isseparator(c))
                    std::cout << c << " separator" << std::endl;
                else if (isoperator(c)) {
                    token += c;
                    input.get(c);
                    if (isoperator(c))
                        token += c;
                    else
                        input.unget();
                    std::cout << token << " operator" << std::endl;
                    token = "";
                }

		}
	}
}

int getstate(char c) {
	if (isalpha(c))
		return 1;
	else if (isdigit(c))
		return 2;
	else
		return -1;
}

int numstate(char c) {
	if (isdigit(c))
		return 1;
	else if (c == '.')
		return 2;
	else
		return -1;
}

int idstate(char c) {
	if (isalpha(c))
		return 1;
	else if (c == '#')
		return 2;
	else if (isdigit(c))
		return 3;
	else
		return -1;
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
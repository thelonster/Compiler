#include "compiler.h"
#include "macros.h"
 
//Note to self: use replace case numbers with easy to read macro state names

void lexer(std::ifstream& input) {
	std::string token;
	char c;
	bool floatflag = false, tokenfound = false, hashflag = false, keyflag = false;
	while (input.get(c)) {
		tokenfound = false;
		switch (getstate(c)) {
			//This case is for ID's
			case ID_STATE:
				//state 1 -> 2: adding letter then moving to second state
				token += c;
				while (!tokenfound && input.get(c)) {
                    //This switch is the 2nd state that can move to the 3rd/4th/5th/6th state
					switch (idstate(c)) {
                        //letter state
						case ID_LETTER:
						    token += c;
                            hashflag = false; //if a number is added, reset hashflag
						    //break moves from state 3/6 to state 2
						    break;
                        //# state
                        case ID_POUND:
                            //Can't have 2 # signs in a row
                            if (hashflag) {
                                std::cout << "unknown\t" << token << std::endl;
                                tokenfound = true;
                                break;
                            }
                            token += c;
                            hashflag = true;
                            break;
                        //number state
                        case ID_NUMBER:
                            token += c;
                            hashflag = false;
                            break;
                        //accepting state
                        default:
                            input.unget();
                            tokenfound = true;
                            for (auto it = keywords.begin(); it != keywords.end(); it++) {
                                if (token == *it) {
                                    std::cout << "keyword\t\t" << token << std::endl;
                                    keyflag = true;
                                }
                            }
                            if (!keyflag)
                                std::cout << "identifier\t" << token << std::endl;
                            keyflag = false;
                            token = "";
                            break;
					}
				}
				break;
			//This state is for ints and floats
			case NUMBER_STATE:
				do {
                    //This switch is the 1st state where is can move to the 2nd/3rd/4th state
					switch (numstate(c))	{
						//number state
						case NUM_NUMBER:
							token += c;
							break;
						//period state
						case NUM_PERIOD:
							floatflag = true;
							token += c;
							break;
						//accepting state
						default:
							input.unget();
							if (floatflag)
								std::cout << "float\t\t" << token << std::endl;
							else
								std::cout << "int\t\t" << token << std::endl;
							floatflag = false;
							token = "";
							tokenfound = true;
							break;
					}
				} while (!tokenfound && input.get(c));
				break;
            //state for operators & separators (also blank spaces)
            default:
                if (isseparator(c)) {
                    token += c;
                    input.get(c);
                    if (isseparator(c))
                        token += c;
                    else
                        input.unget();
                    std::cout << "separator\t" << token << std::endl;
                    token = "";
                }
                else if (isoperator(c)) {
                    token += c;
                    input.get(c);
                    if (isoperator(c))
                        token += c;
                    else
                        input.unget();
                    std::cout << "operator\t" << token << std::endl;
                    token = "";
                }
                break;
		}
	}
}

int getstate(char c) {
	if (isalpha(c))
		return ID_STATE;
	else if (isdigit(c))
		return NUMBER_STATE;
	else
		return -1;
}

int numstate(char c) {
	if (isdigit(c))
		return NUM_NUMBER;
	else if (c == '.')
		return NUM_PERIOD;
	else
		return -1;
}

int idstate(char c) {
	if (isalpha(c))
		return ID_LETTER;
	else if (c == '#')
		return ID_POUND;
	else if (isdigit(c))
		return ID_NUMBER;
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

int main() {
    std::ifstream input;
    input.open("C:\\Users\\Lonnie\\Source\\Repos\\Compiler\\Compiler\\test.txt");
    std::string token;
    std::string lexeme;
    std::cout << "Token\t\tLexeme" << std::endl;
    lexer(input);
    input.close();
    return 0;
}
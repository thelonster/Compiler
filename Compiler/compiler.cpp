#include "compiler.h"
#include "macros.h"

Token lexer(std::ifstream& input, char c) {
    std::string lexeme;
    Token token;
    bool floatflag = false, tokenfound = false, hashflag = false;
    switch (getstate(c)) {
        //This case is for ID's
        case ID_STATE:
            //state 1 -> 2: adding letter then moving to second state
            lexeme += c;
            while (!tokenfound && input.get(c)) {
                //This switch is the 2nd state that can move to the 3rd/4th/5th/6th state
                switch (idstate(c)) {
                    //letter state
	                case ID_LETTER:
	                    lexeme += c;
                        hashflag = false; //if a number is added, reset hashflag
	                    //break moves from state 3/6 to state 2
	                    break;
                    //# state
                    case ID_POUND:
                        //Can't have 2 # signs in a row
                        if (hashflag) {
                            token.lexeme = lexeme;
                            token.token = "unknown";
                            return token; //"unknown\t" + lexeme;
                            tokenfound = true;
                            break;
                        }
                        lexeme += c;
                        hashflag = true;
                        break;
                    //number state
                    case ID_NUMBER:
                        lexeme += c;
                        hashflag = false;
                        break;
                    //accepting state
                    default:
                        input.unget();
                        tokenfound = true;
                        for (auto it = keywords.begin(); it != keywords.end(); it++) {
                            if (lexeme == *it) {
                                token.lexeme = lexeme;
                                token.token = "keyword";
                                return token; //"keyword\t\t" + lexeme;
                            }
                        }
                        token.lexeme = lexeme;
                        token.token = "identifier";
                        return token; //"identifier\t" + lexeme;
                        break;
                }
            }
            break;
        //This state is for ints and floats
        case NUMBER_STATE:
            do {
                //This switch is the 1st state where is can move to the 2nd/3rd/4th state
                switch (numstate(c)) {
                    //number state
                    case NUM_NUMBER:
                        lexeme += c;
                        break;
                    //period state
                    case NUM_PERIOD:
                        floatflag = true;
                        lexeme += c;
                        break;
                    //accepting state
                    default:
                        input.unget();
                        if (floatflag) {
                            token.lexeme = lexeme;
                            token.token = "real";
                            return token; //"real\t\t" + lexeme;
                        }                            
                        else {
                            token.lexeme = lexeme;
                            token.token = "int";
                            return token; //"int\t\t" + lexeme;
                        }
                        break;
                }
            } while (!tokenfound && input.get(c));
            break;
        //state for operators & separators (also blank spaces)
        default:
            if (isseparator(c)) {
                lexeme += c;
                input.get(c);
                if (isseparator(c))
                    lexeme += c;
                else
                    input.unget();
                token.lexeme = lexeme;
                token.token = "separator";
                return token; //"separator\t" + lexeme;
            }
            else if (isoperator(c)) {
                lexeme += c;
                input.get(c);
                if (isoperator(c))
                    lexeme += c;
                else
                    input.unget();
                token.lexeme = lexeme;
                token.token = "operator";
                return token; //"operator\t" + lexeme;
            }
            else {
                token.lexeme = "";
                token.token = "";
                return token;
            }
            break;
    }
}

int getstate(char c) {
    if (isletter(c))
        return ID_STATE;
    else if (isnumber(c))
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

bool isletter(char c) {
    for (auto it = letters.begin(); it != letters.end(); it++)
        if (*it == c)
            return true;
    return false;
}

bool isnumber(char c) {
    for (auto it = numbers.begin(); it != numbers.end(); it++)
        if (*it == c)
            return true;
    return false;
}

int main() {
    std::ifstream input;
    input.open("C:\\Users\\Lonnie\\Source\\Repos\\Compiler\\Compiler\\test.txt");
    char c;
    Token token;
    std::cout << std::setw(16) << std::left <<"Token" << "Lexeme" << std::endl;
    while (input.get(c)) {
        token = lexer(input, c);
        if (token.lexeme != "")
            std::cout << std::setw(16) << std::left << token.token << token.lexeme << std::endl;
    }
    input.close();
    return 0;
}
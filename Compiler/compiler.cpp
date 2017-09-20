#include "compiler.h"
#include "macros.h"

Token lexer(std::ifstream& input, char c) {
    std::string lexeme;
    Token token;
    bool tokenfound = false;
    switch (getstate(c)) {
        //This case is for ID's
        case ID_STATE:
            //state 1 -> 2: adding letter then moving to second state
            lexeme += c;
            while (!tokenfound && input.get(c)) {
                //This switch is the 2nd state that can move to the 3rd/5th/8th/9th state
                switch (idstate(c)) {
                    //letter state
	                case ID_LETTER:
	                    lexeme += c;
	                    break;
                    //# state
                    case ID_POUND:
                        lexeme += c;
                        if (input.get(c) && isletter(c)) {
                            lexeme += c;
                        }
                        else {
                            input.unget();
                            tokenfound = true;
                            token.lexeme = lexeme;
                            token.token = "identifier";
                            tokenfound = true;// return token;
                        }
                        break;
                    //accepting state
                    default:
                        input.unget();
                        tokenfound = true;
                        for (auto it = keywords.begin(); it != keywords.end(); it++) {
                            if (lexeme == *it) {
                                token.lexeme = lexeme;
                                token.token = "keyword";
                                tokenfound = true;// return token; //"keyword\t\t" + lexeme;
                            }
                        }
                        if (token.token != "keyword") {
                            token.lexeme = lexeme;
                            token.token = "identifier";
                        }
                        tokenfound = true;
                        //return token; //"identifier\t" + lexeme;
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
                        lexeme += c;
                        while (!tokenfound && input.get(c)) {
                            switch (floatstate(c)) {
                                case FLOAT_NUMBER:
                                    lexeme += c;
                                    break;
                                default:
                                    input.unget();
                                    token.lexeme = lexeme;
                                    token.token = "real";
                                    tokenfound = true;
                            }
                        }
                        break;
                    //accepting state
                    default:
                        input.unget();
                        token.lexeme = lexeme;
                        token.token = "int";
                        tokenfound = true;
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
                tokenfound = true; //return token; //"separator\t" + lexeme;
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
                tokenfound = true;// return token; //"operator\t" + lexeme;
            }
            else {
                token.lexeme = "";
                token.token = "";
                tokenfound = true;// return token;
            }
            break;
    }
    return token;
}

int getstate(char c) {
    if (isletter(c))
        return ID_STATE;
    else if (isnumber(c))
        return NUMBER_STATE;
    else if (isoperator(c))
        return OP_STATE;
    else if (isseparator(c))
        return SEP_STATE;
    else
        return EXIT_STATE;
}

int numstate(char c) {
    if (isdigit(c))
        return NUM_NUMBER;
    else if (c == '.')
        return NUM_PERIOD;
    else
        return EXIT_STATE;
}

int idstate(char c) {
    if (isletter(c))
        return ID_LETTER;
    else if (c == '#')
        return ID_POUND;
    else
        return EXIT_STATE;
}

int floatstate(char c) {
    if (isnumber(c))
        return FLOAT_NUMBER;
    else
        return EXIT_STATE;
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
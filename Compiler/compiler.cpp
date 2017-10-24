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
                            tokenfound = true;
                        }
                        break;
                    //accepting state
                    default:
                        input.unget();
                        tokenfound = true;
                        if (iskeyword(lexeme)) {
                            token.lexeme = lexeme;
                            token.token = "keyword";
                        }
                        else { //if (token.token != "keyword") {
                            token.lexeme = lexeme;
                            token.token = "identifier";
                        }
                        tokenfound = true;
                        break;
                }
            }
            break;
        //This state is for ints and floats
        case NUMBER_STATE:
            //instead of d+, my FSM is dd*, which is equivalent to d+
            lexeme += c; //state 1 -> 2
            while (!tokenfound && input.get(c)) {
                //This switch is the 2nd state where is can move to the 3rd/4th/5th states
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
            } 
            break;
        //state for operators & separators (also blank spaces)
        case OP_STATE:
            lexeme += c;
            input.get(c);
            if (isoperator(c)) {
                lexeme += c;
                if (!isoperator(lexeme)) {
                    lexeme.pop_back();
                    input.unget();
                }
            }
            else
                input.unget();
            token.lexeme = lexeme;
            token.token = "operator";
            break;
        case SEP_STATE:
            lexeme += c;
            input.get(c);
            if (isseparator(c)) {
                lexeme += c;
                if (!isseparator(lexeme)) {
                    lexeme.pop_back();
                    input.unget();
                }
            }
            else
                input.unget();
            token.lexeme = lexeme;
            token.token = "separator";
            break;
        default:
            if (isspace(c)) {
                token.lexeme = "";
                token.token = "";
            }
            else {
                token.lexeme = c;
                token.token = "unknown";
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
    if (isnumber(c))
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

bool isseparator(std::string s) {
    for (auto it = multiseparators.begin(); it != multiseparators.end(); it++)
        if (*it == s)
            return true;
    return false;
}

bool isoperator(char c) {
    for (auto it = operators.begin(); it != operators.end(); it++)
        if (*it == c)
            return true;
    return false;
}

bool isoperator(std::string s) {
    for (auto it = multioperators.begin(); it != multioperators.end(); it++)
        if (*it == s)
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

bool iskeyword(std::string s) {
    for (auto it = keywords.begin(); it != keywords.end(); it++)
        if (*it == s)
            return true;
    return false;
}

int nonterminalindex(std::string nt) {
    if (nt == "RAT") { return 0; }
    else if (nt == "OPT_FD") { return 1; }
    else if (nt == "FD") { return 2; }
    else if (nt == "FD'") { return 3; }
    else if (nt == "F") { return 4; }
    else if (nt == "OPT_PL") { return 5; }
    else if (nt == "PL") { return 6; }
    else if (nt == "PL'") { return 7; }
    else if (nt == "P") { return 8; }
    else if (nt == "Q") { return 9; }
    else if (nt == "B") { return 10; }
    else if (nt == "OPT_DL") { return 11; }
    else if (nt == "DL") { return 12; }
    else if (nt == "DL'") { return 13; }
    else if (nt == "D") { return 14; }
    else if (nt == "ID") { return 15; }
    else if (nt == "ID'") { return 16; }
    else if (nt == "SL") { return 17; }
    else if (nt == "SL'") { return 18; }
    else if (nt == "S") { return 19; }
    else if (nt == "COM") { return 20; }
    else if (nt == "A") { return 21; }
    else if (nt == "IF") { return 22; }
    else if (nt == "IF'") { return 23; }
    else if (nt == "RET") { return 24; }
    else if (nt == "RET'") { return 25; }
    else if (nt == "WR") { return 26; }
    else if (nt == "RD") { return 27; }
    else if (nt == "WH") { return 28; }
    else if (nt == "COND") { return 29; }
    else if (nt == "REL") { return 30; }
    else if (nt == "EX") { return 31; }
    else if (nt == "EX'") { return 32; }
    else if (nt == "T") { return 33; }
    else if (nt == "T'") { return 34; }
    else if (nt == "FCT") { return 35; }
    else if (nt == "PRI") { return 36; }
    else if (nt == "EPS") { return 37; }
    else { return -1; }
}

int terminalindex(std::string t) {
    if (t == "@") { return 0; }
    else if (t == "id") { return 1; }
    else if (t == "integer") { return 2; }
    else if (t == "boolean") { return 3; }
    else if (t == "real") { return 4; }
    else if (t == "{") { return 5; }
    else if (t == "if") { return 6; }
    else if (t == "fi") { return 7; }
    else if (t == "else") { return 8; }
    else if (t == "return") { return 9; }
    else if (t == "write") { return 10; }
    else if (t == "read") { return 11; }
    else if (t == "while") { return 12; }
    else if (t == "-") { return 13; }
    else if (t == "+") { return 14; }
    else if (t == "*") { return 15; }
    else if (t == "/") { return 16; }
    else if (t == "=") { return 17; }
    else if (t == "/=") { return 18; }
    else if (t == "=>") { return 19; }
    else if (t == "<=") { return 20; }
    else if (t == "<") { return 21; }
    else if (t == ">") { return 22; }
    else if (t == "(") { return 23; }
    else if (t == "true") { return 24; }
    else if (t == "false") { return 25; }
    else if (t == ",") { return 26; }
    else if (t == "%%") { return 27; }
    else if (t == ")") { return 28; }
    else if (t == ";") { return 29; }
    else if (t == "e") { return 30; }
    else if (t == "$") { return 31; }
    else { return -1; }
}

int main(int argc, const char* argv[]) {
    std::ifstream input;
    if (argc < 2) {
        std::cout << "Please specify a text file to run through the lexical analyzer" << std::endl;
        return 0;
    }
    for (int i = 1; i < argc; i++) {
        input.open(argv[i]);
        if (!input.is_open()) {
            std::cout << "Please enter a correct file name to run through the lexical analyzer" << std::endl;
            break;
        }
        char c;
        Token token;
        std::cout << std::endl << "========== Running " << argv[i] << " through the lexer ==========" << std::endl << std::endl;
        std::cout << std::setw(16) << std::left << "Token" << "Lexeme" << std::endl;
        while (input.get(c)) {
            token = lexer(input, c);
            if (token.lexeme != "")
                std::cout << std::setw(16) << std::left << token.token << token.lexeme << std::endl;
        }
        input.close();
    }
    return 0;
}
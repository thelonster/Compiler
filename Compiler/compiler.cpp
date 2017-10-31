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
                            token.token = "id";
                            token.lineno = linenumber;
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
                            token.lineno = linenumber;
                        }
                        else { //if (token.token != "keyword") {
                            token.lexeme = lexeme;
                            token.token = "id";
                            token.lineno = linenumber;
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
                                    token.lineno = linenumber;
                                    tokenfound = true;
                            }
                        }
                        break;
                    //accepting state
                    default:
                        input.unget();
                        token.lexeme = lexeme;
                        token.token = "integer";
                        token.lineno = linenumber;
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
            token.lineno = linenumber;
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
            token.lineno = linenumber;
            break;
        default:
            if (isspace(c)) {
                if (c == '\n')
                    linenumber++;
                token.lexeme = "";
                token.token = "";
                token.lineno = linenumber;
            }
            else {
                token.lexeme = c;
                token.token = "unknown";
                token.lineno = linenumber;
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
    if (nt == "<RAT>") { return 0; }
    else if (nt == "<OPT_FD>") { return 1; }
    else if (nt == "<FD>") { return 2; }
    else if (nt == "<FD'>") { return 3; }
    else if (nt == "<F>") { return 4; }
    else if (nt == "<OPT_PL>") { return 5; }
    else if (nt == "<PL>") { return 6; }
    else if (nt == "<PL'>") { return 7; }
    else if (nt == "<P>") { return 8; }
    else if (nt == "<Q>") { return 9; }
    else if (nt == "<B>") { return 10; }
    else if (nt == "<OPT_DL>") { return 11; }
    else if (nt == "<DL>") { return 12; }
    else if (nt == "<DL'>") { return 13; }
    else if (nt == "<D>") { return 14; }
    else if (nt == "<ID>") { return 15; }
    else if (nt == "<ID'>") { return 16; }
    else if (nt == "<SL>") { return 17; }
    else if (nt == "<SL'>") { return 18; }
    else if (nt == "<S>") { return 19; }
    else if (nt == "<COM>") { return 20; }
    else if (nt == "<A>") { return 21; }
    else if (nt == "<IF>") { return 22; }
    else if (nt == "<IF'>") { return 23; }
    else if (nt == "<RET>") { return 24; }
    else if (nt == "<RET'>") { return 25; }
    else if (nt == "<WR>") { return 26; }
    else if (nt == "<RD>") { return 27; }
    else if (nt == "<WH>") { return 28; }
    else if (nt == "<COND>") { return 29; }
    else if (nt == "<REL>") { return 30; }
    else if (nt == "<EX>") { return 31; }
    else if (nt == "<EX'>") { return 32; }
    else if (nt == "<T>") { return 33; }
    else if (nt == "<T'>") { return 34; }
    else if (nt == "<FCT>") { return 35; }
    else if (nt == "<PRI>") { return 36; }
    else if (nt == "<EPS>") { return 37; }
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
    else if (t == ":") { return 31; }
    else if (t == "]") { return 32; }
    else if (t == "}") { return 33; }
    else if (t == "[") { return 34; }
    else if (t == "$") { return 35; }
    else { return -1; }
}

bool isnonterminal(std::string st) {
    if (st[0] == '<' && st.size() > 1)
        return true;
    return false;
}

std::string getterminal(int index) {
    if (index == 0) { return "@"; }
    else if (index == 1) { return "id"; }
    else if (index == 2) { return "integer"; }
    else if (index == 3) { return "boolean"; }
    else if (index == 4) { return "real"; }
    else if (index == 5) { return "{"; }
    else if (index == 6) { return "if"; }
    else if (index == 7) { return "fi"; }
    else if (index == 8) { return "else"; }
    else if (index == 9) { return "return"; }
    else if (index == 10) { return "write"; }
    else if (index == 11) { return "read"; }
    else if (index == 12) { return "while"; }
    else if (index == 13) { return "-"; }
    else if (index == 14) { return "+"; }
    else if (index == 15) { return "*"; }
    else if (index == 16) { return "/"; }
    else if (index == 17) { return "="; }
    else if (index == 18) { return "/="; }
    else if (index == 19) { return "=>"; }
    else if (index == 20) { return "<="; }
    else if (index == 21) { return "<"; }
    else if (index == 22) { return ">"; }
    else if (index == 23) { return "("; }
    else if (index == 24) { return "true"; }
    else if (index == 25) { return "false"; }
    else if (index == 26) { return ","; }
    else if (index == 27) { return "%%"; }
    else if (index == 28) { return ")"; }
    else if (index == 29) { return ";"; }
    else if (index == 30) { return "e"; }
    else if (index == 31) { return ":"; }
    else if (index == 32) { return "]"; }
    else if (index == 33) { return "}"; }
    else if (index == 34) { return "["; }
    else if (index == 35) { return "$"; }
    else { return "\0"; }
}

void filltable() {
    for (int l = 0; l < 38; l++)
        for (int w = 0; w < 35; w++)
            for (int h = 0; h < 7; h++)
                table[l][w][h] = "";
    table[0][0][0] = "<OPT_FD>";
    table[0][0][1] = "%%";
    table[0][0][2] = "<OPT_DL>";
    table[0][0][3] = "<SL>";
    table[0][35][0] = "e";
    table[1][0][0] = "<FD>";
    table[1][27][0] = "e";
    table[1][30][0] = "<EPS>";
    table[2][0][0] = "<F>";
    table[2][0][1] = "<FD'>";
    table[3][0][0] = "<FD>";
    table[3][27][0] = "e";
    table[3][28][0] = "e";
    table[3][30][0] = "<EPS>";
    table[4][0][0] = "@";
    table[4][0][1] = "id";
    table[4][0][2] = "(";
    table[4][0][3] = "<OPT_PL>";
    table[4][0][4] = ")";
    table[4][0][5] = "<OPT_DL>";
    table[4][0][6] = "<B>";
    table[5][1][0] = "<PL>";
    table[5][28][0] = "e";
    table[5][30][0] = "<EPS>";
    table[6][1][0] = "<P>";
    table[6][1][1] = "<PL'>";
    table[7][26][0] = ",";
    table[7][26][1] = "<PL>";
    table[7][28][0] = "e";
    table[7][30][0] = "<EPS>";
    table[8][1][0] = "<ID>";
    table[8][1][1] = ":";
    table[8][1][2] = "<Q>";
    table[9][2][0] = "integer";
    table[9][3][0] = "boolean";
    table[9][4][0] = "real";
    table[10][5][0] = "{";
    table[10][5][1] = "<SL>";
    table[10][5][2] = "}";
    table[11][1][0] = "e";
    table[11][2][0] = "<DL>";
    table[11][3][0] = "<DL>";
    table[11][4][0] = "<DL>";
    table[11][5][0] = "e";
    table[11][6][0] = "e";
    table[11][9][0] = "e";
    table[11][10][0] = "e";
    table[11][11][0] = "e";
    table[11][12][0] = "e";
    table[11][30][0] = "<EPS>";
    table[12][2][0] = "<D>";
    table[12][2][1] = ";";
    table[12][2][2] = "<DL'>";
    table[12][3][0] = "<D>";
    table[12][3][1] = ";";
    table[12][3][2] = "<DL'>";
    table[12][4][0] = "<D>";
    table[12][4][1] = ";";
    table[12][4][2] = "<DL'>";
    table[13][1][0] = "e";
    table[13][2][0] = "<DL>";
    table[13][3][0] = "<DL>";
    table[13][4][0] = "<DL>";
    table[13][5][0] = "e";
    table[13][6][0] = "e";
    table[13][9][0] = "e";
    table[13][10][0] = "e";
    table[13][11][0] = "e";
    table[13][12][0] = "e";
    table[13][30][0] = "<EPS>";
    table[14][2][0] = "<Q>";
    table[14][2][1] = "<ID>";
    table[14][3][0] = "<Q>";
    table[14][3][1] = "<ID>";
    table[14][4][0] = "<Q>";
    table[14][4][1] = "<ID>";
    table[15][1][0] = "id";
    table[15][1][1] = "<ID'>";
    table[16][26][0] = ",";
    table[16][26][1] = "<ID>";
    table[16][13][0] = "e";
    table[16][14][0] = "e";
    table[16][17][0] = "e";
    table[16][18][0] = "e";
    table[16][19][0] = "e";
    table[16][20][0] = "e";
    table[16][21][0] = "e";
    table[16][22][0] = "e";
    table[16][28][0] = "e";
    table[16][29][0] = "e";
    table[16][30][0] = "<EPS>";
    table[16][31][0] = "e";
    table[16][32][0] = "e";
    table[16][34][0] = "[";
    table[16][34][1] = "<ID>";
    table[16][34][2] = "]";
    table[17][1][0] = "<S>";
    table[17][1][1] = "<SL'>";
    table[17][5][0] = "<S>";
    table[17][5][1] = "<SL'>";
    table[17][6][0] = "<S>";
    table[17][6][1] = "<SL'>";
    table[17][9][0] = "<S>";
    table[17][9][1] = "<SL'>";
    table[17][10][0] = "<S>";
    table[17][10][1] = "<SL'>";
    table[17][11][0] = "<S>";
    table[17][11][1] = "<SL'>";
    table[17][12][0] = "<S>";
    table[17][12][1] = "<SL'>";
    table[18][1][0] = "<SL>";
    table[18][5][0] = "<SL>";
    table[18][6][0] = "<SL>";
    table[18][9][0] = "<SL>";
    table[18][10][0] = "<SL>";
    table[18][11][0] = "<SL>";
    table[18][12][0] = "<SL>";
    table[18][30][0] = "<EPS>";
    table[18][33][0] = "e";
    table[18][35][0] = "e";
    table[19][1][0] = "<A>";
    table[19][5][0] = "<COM>";
    table[19][6][0] = "<IF>";
    table[19][9][0] = "<RET>";
    table[19][10][0] = "<WR>";
    table[19][11][0] = "<RD>";
    table[19][12][0] = "<WH>";
    table[20][5][0] = "{";
    table[20][5][1] = "<SL>";
    table[20][5][2] = "}";
    table[21][1][0] = "id";
    table[21][1][1] = ":=";
    table[21][1][2] = "<EX>";
    table[21][1][3] = ";";
    table[22][6][0] = "if";
    table[22][6][1] = "(";
    table[22][6][2] = "<COND>";
    table[22][6][3] = ")";
    table[22][6][4] = "<S>";
    table[22][6][5] = "<IF'>";
    table[23][7][0] = "fi";
    table[23][8][0] = "else";
    table[23][8][1] = "<S>";
    table[23][8][2] = "fi";
    table[24][9][0] = "return";
    table[24][9][1] = "<RET'>";
    table[25][1][0] = "<EX>";
    table[25][1][1] = ";";
    table[25][2][0] = "<EX>";
    table[25][2][1] = ";";
    table[25][4][0] = "<EX>";
    table[25][4][1] = ";";
    table[25][13][0] = "<EX>";
    table[25][13][1] = ";";
    table[25][23][0] = "<EX>";
    table[25][23][1] = ";";
    table[25][24][0] = "<EX>";
    table[25][24][1] = ";";
    table[25][25][0] = "<EX>";
    table[25][25][1] = ";";
    table[26][10][0] = "write";
    table[26][10][1] = "(";
    table[26][10][2] = "<EX>";
    table[26][10][3] = ")";
    table[26][10][4] = ";";
    table[27][11][0] = "read";
    table[27][11][1] = "(";
    table[27][11][2] = "<ID>";
    table[27][11][3] = ")";
    table[27][11][4] = ";";
    table[28][12][0] = "while";
    table[28][12][1] = "(";
    table[28][12][2] = "<COND>";
    table[28][12][3] = ")";
    table[28][12][4] = "<S>";
    table[29][1][0] = "<EX>";
    table[29][1][1] = "<REL>";
    table[29][1][2] = "<EX>";
    table[29][2][0] = "<EX>";
    table[29][2][1] = "<REL>";
    table[29][2][2] = "<EX>";
    table[29][3][0] = "<EX>";
    table[29][3][1] = "<REL>";
    table[29][3][2] = "<EX>";
    table[29][4][0] = "<EX>";
    table[29][4][1] = "<REL>";
    table[29][4][2] = "<EX>";
    table[29][13][0] = "<EX>";
    table[29][13][1] = "<REL>";
    table[29][13][2] = "<EX>";
    table[29][23][0] = "<EX>";
    table[29][23][1] = "<REL>";
    table[29][23][2] = "<EX>";
    table[29][24][0] = "<EX>";
    table[29][24][1] = "<REL>";
    table[29][24][2] = "<EX>";
    table[29][25][0] = "<EX>";
    table[29][25][1] = "<REL>";
    table[29][25][2] = "<EX>";
    table[30][17][0] = "=";
    table[30][18][0] = "/=";
    table[30][19][0] = "=>";
    table[30][20][0] = "<=";
    table[30][21][0] = "<";
    table[30][22][0] = ">";
    table[31][1][0] = "<T>";
    table[31][1][1] = "<EX'>";
    table[31][2][0] = "<T>";
    table[31][2][1] = "<EX'>";
    table[31][3][0] = "<T>";
    table[31][3][1] = "<EX'>";
    table[31][4][0] = "<T>";
    table[31][4][1] = "<EX'>";
    table[31][13][0] = "<T>";
    table[31][13][1] = "<EX'>";
    table[31][23][0] = "<T>";
    table[31][23][1] = "<EX'>";
    table[31][24][0] = "<T>";
    table[31][24][1] = "<EX'>";
    table[31][25][0] = "<T>";
    table[31][25][1] = "<EX'>";
    table[32][13][0] = "-";
    table[32][13][1] = "<T>";
    table[32][13][2] = "<EX'>";
    table[32][14][0] = "+";
    table[32][14][1] = "<T>";
    table[32][14][2] = "<EX'>";
    table[32][17][0] = "e";
    table[32][18][0] = "e";
    table[32][19][0] = "e";
    table[32][20][0] = "e";
    table[32][21][0] = "e";
    table[32][22][0] = "e";
    table[32][28][0] = "e";
    table[32][29][0] = "e";
    table[32][30][0] = "<EPS>";
    table[33][1][0] = "<FCT>";
    table[33][1][1] = "<T'>";
    table[33][2][0] = "<FCT>";
    table[33][2][1] = "<T'>";
    table[33][3][0] = "<FCT>";
    table[33][3][1] = "<T'>";
    table[33][4][0] = "<FCT>";
    table[33][4][1] = "<T'>";
    table[33][13][0] = "<FCT>";
    table[33][13][1] = "<T'>";
    table[33][23][0] = "<FCT>";
    table[33][23][1] = "<T'>";
    table[33][24][0] = "<FCT>";
    table[33][24][1] = "<T'>";
    table[33][25][0] = "<FCT>";
    table[33][25][1] = "<T'>";
    table[34][13][0] = "e";
    table[34][14][0] = "e";
    table[34][15][0] = "*";
    table[34][15][1] = "<FCT>";
    table[34][15][2] = "<T'>";
    table[34][16][0] = "/";
    table[34][16][1] = "<FCT>";
    table[34][16][2] = "<T'>";
    table[34][17][0] = "e";
    table[34][18][0] = "e";
    table[34][19][0] = "e";
    table[34][20][0] = "e";
    table[34][21][0] = "e";
    table[34][22][0] = "e";
    table[34][28][0] = "e";
    table[34][29][0] = "e";
    table[34][30][0] = "<EPS>";
    table[35][1][0] = "<PRI>";
    table[35][2][0] = "<PRI>";
    table[35][3][0] = "<PRI>";
    table[35][4][0] = "<PRI>";
    table[35][13][0] = "-";
    table[35][13][1] = "<PRI>";
    table[35][23][0] = "<PRI>";
    table[35][24][0] = "<PRI>";
    table[35][25][0] = "<PRI>";
    table[36][1][0] = "id";
    table[36][1][1] = "<ID'>";
    table[36][2][0] = "integer";
    table[36][3][0] = "boolean";
    table[36][4][0] = "real";
    table[36][23][0] = "(";
    table[36][23][1] = "<EX>";
    table[36][23][2] = ")";
    table[36][24][0] = "true";
    table[36][24][0] = "false";
    table[37][30][0] = "e";
    table[37][35][0] = "e";
}

int lastprodindex(int r, int c) {
    int index = 0;
    for (int a = 0; a < 7; a++) {
        if (table[r][c][a] != "")
            index = a;
        else
            break;
    }
    return index;
}

void syntaxerdriver(std::string filename) {
    filltable();
    TDPPstack.push("$");
    std::vector<Token> inputstring;
    char ch = 0;
    std::ifstream input;
    input.open(filename);// "C:/Users/Lonnie/Source/Repos/Compiler/Compiler/test.txt");
    linenumber = 0;
    //Adding tokens to a vector so that I don't need to worry about ch and input later.    
    while (input.get(ch)) {
        Token temptok = lexer(input, ch);
        if (temptok.lexeme != "")
            inputstring.push_back(temptok);
    }    
    Token temp;
    temp.lexeme = "$";
    temp.token = "EOF";
    inputstring.push_back(temp);
    int index = 0;
    TDPPstack.push("<RAT>");
    Token i = inputstring[index];
    std::cout << "Token: " << i.token << " Lexeme: " << i.lexeme << std::endl;
    std::string t;
    while (TDPPstack.top() != "$") {
        t = TDPPstack.top();
        if (!isnonterminal(t)) {
            if (t == i.lexeme || t == i.token) {
                TDPPstack.pop();
                i = inputstring[++index];               
                std::cout << "Token: " << std::setw(10) << std::left << i.token << " Lexeme: " << i.lexeme << std::endl;
            }
            else {
                //If the top of the stack is epsilon, pop and continue
                if (t == "e") {
                    TDPPstack.pop();
                    continue;
                }
                std::cout << "Line " << i.lineno << " Expected " << t << " but got " << i.lexeme << " instead" << std::endl;
                return;
            }
        }
        else {
            //gets the row of the non-terminal
            int r = nonterminalindex(t);
            //gets the row of the terminal if the terminal character is a keyword, operator, or separator
            int c = terminalindex(i.lexeme);
            if (c == -1)
                //if the terminal is an integer, real, or identifier, get the index of the token type instead
                c = terminalindex(i.token);
            if (table[r][c][0] != "") {
                int lpi = lastprodindex(r, c);
                //prints out the production that corresponds with the token
                std::cout << t << " ->";
                for (int a = 0; a <= lpi; a++)
                    std::cout << " " << table[r][c][a];
                std::cout << std::endl;
                TDPPstack.pop();
                for (int h = lpi; h >= 0; h--)
                    TDPPstack.push(table[r][c][h]);
            }
            else {
                //Prints out error message
                std::cout << "Line " << i.lineno << " Unexpected token found: " << i.lexeme << " Expecting: ";
                //Prints out expected tokens based on first set of production
                for (int a = 0; a < 35; a++)
                    if (table[r][a][0] != "")
                        std::cout << getterminal(a) << "|";
                std::cout << '\b' << " " << std::endl;
                return;
            }
        }
    }
    std::cout << "Stack empty, Syntax correct!" << std::endl;
    input.close();
}

int main(int argc, const char* argv[]) {
    std::ifstream input;
    if (argc < 2) {
        std::cout << "Please specify a text file to run through the syntax analyzer" << std::endl;
        return 0;
    }
    for (int i = 1; i < argc; i++) {
        input.open(argv[i]);
        if (!input.is_open()) {
            std::cout << "Please enter a correct file name to run through the syntax analyzer" << std::endl;
            break;
        }
        linenumber = 0;
        char c;
        Token token;
        std::cout << std::endl << "========== Running " << argv[i] << " through the syntaxer ==========" << std::endl << std::endl;
        syntaxerdriver(argv[i]);
    }
    return 0;
}
#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <stack>

struct Token {
    std::string token;
    std::string lexeme;
    int lineno = 0;
};

struct Instruction {
    int instaddr;
    std::string oper8r;
    int operand;
};

struct SymbolTable {
    int address;
    Token tok;
    std::string type;
};

int linenumber, instr_address, addr = 0, oprnd_addr = 3000;
std::vector<std::string> keywords = { "while", "if", "int", "fi", "else", "return", "read", "write", "integer", "real", "boolean", "true", "false" };
std::vector<char> separators = { '(', ')', '{', '}', '%', '@', '[', ']', ';', ',', '.' };
std::vector<std::string> multiseparators = { "%%" };
std::vector<char> operators = { '+', '-', '/', '*', '<', '>', '=', ':', '|' };
std::vector<std::string> multioperators = { "+=", "-=", "*=", "/=", "==", "<=", ">=", ":=", "&&", "||" };
std::vector<char> letters = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                              'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
std::vector<char> numbers = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
std::stack<std::string> TDPPstack;
std::string table[38][36][7];
Instruction instrtable[500];
std::vector<SymbolTable> symbol_table;
std::stack<int> jump_stack;

/*Lexical Analyzer function that currently just prints tokens/lexemes*/
Token lexer(std::ifstream& input, char c);
/*Returns whether to go to ID, int/real, or the other states*/
int getstate(char c);
/*Returns whether a number, period, or other character is found and goes to that state*/
int numstate(char c);
/*Returns whether a letter, or '#' is found and goes to that state*/
int idstate(char c);
/*Returns whether a number or other character is found and goes to that state*/
int floatstate(char c);
/*Returns true if character/string is a separator as defined in vectors above*/
bool isseparator(char c);
bool isseparator(std::string s); //Compares string with multiseparator vector
/*Returns true if character/string is a operator as defined in vectors above*/
bool isoperator(char c);
bool isoperator(std::string); //Compares string with multioperator vector
/*Returns true if character is a letter as defined in vector above*/
bool isletter(char c);
/*Returns true if character is a number as defined in vector above*/
bool isnumber(char c);
/*Returns true if string is a keyword*/
bool iskeyword(std::string s);
/*Converts non-terminal character to index in the table*/
int nonterminalindex(std::string nt);
/*Converts terminal character to index in the table*/
int terminalindex(std::string t);
/*Returns whether the string is a non-terminal character or not*/
bool isnonterminal(std::string st);
/*Returns a terminal character from an index*/
std::string getterminal(int index);
/*Fills table with correct values*/
void filltable();
/*Returns the index of the last production rule at [r][c] in the 3D table*/
int lastprodindex(int r, int c);
/*Syntax analyzer driver for Table Driver Predictive Parser*/
void syntaxerdriver(std::string filename);
/*Procedure for ICG*/
void gen_instr(std::string op, int opernd);
/*Gets address of token from Symbol Table*/
int get_address(std::string token);
/*Adds address to jump_stack so it can be back patched*/
void back_patch(int jump_addr);
/*Prints out the instruction table*/
void print_instr_table();
/*Checks if the symbol is in the table, returns it's index*/
int symbol_table_lookup(Token );
/*Prints out the symbol table*/
void print_sym_table();
/*runs the syntaxer*/
int main(int argc, const char * argv[]);

#endif // !COMPILER_H

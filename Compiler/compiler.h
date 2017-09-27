#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

struct Token {
    std::string token;
    std::string lexeme;
};

std::vector<std::string> keywords = { "while", "if", "int", "fi", "else", "return", "read", "write", "integer" };
std::vector<char> separators = { '(', ')', '{', '}', '%', '@', '[', ']', ';', ',', '.' };
std::vector<std::string> multiseparators = { "%%" };
std::vector<char> operators = { '+', '-', '/', '*', '<', '>', '=', ':', '!' };
std::vector<std::string> multioperators = { "+=", "-=", "*=", "/=", "==", "<=", ">=" };
std::vector<char> letters = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
std::vector<char> numbers = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

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
/*runs the lexer*/
int main(int argc, const char * argv[]);
#endif // !COMPILER_H

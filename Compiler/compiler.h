#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

std::vector<std::string> keywords = { "while", "if", "int", "fi", "else", "return", "read", "write" };
std::vector<char> separators = { '(', ')', '{', '}', '%', '@' };
std::vector<char> operators = { '+', '-', '/', '*', '<', '>', '=', ':', '!' };

/*Lexical Analyzer function that currently just prints tokens/lexemes*/
void lexer(std::ifstream& input);
/*Returns whether to go to ID, int/real, or the other states*/
int getstate(char c);
/*Returns whether a number, period, or other character is found and goes to that state*/
int numstate(char c);
/*Returns whether a letter, '#', or number is found and goes to that state*/
int idstate(char c);
/*Returns true if character value is a separator as defined in vector above*/
bool isseparator(char c);
/*Returns true if character value is a operator as defined in vector above*/
bool isoperator(char c);
/*runs the lexer*/
int main();
#endif // !COMPILER_H
#ifndef compiler_h
#define compiler_h

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

/*Lexical Analyzer function that currently just prints tokens/lexemes*/
void lexer(std::ifstream& input);
/*Returns whether to go to ID, int/real, or the other states*/
int getstate(char c);
/*Returns whether a number, period, or other character is found and goes to that state*/
int numstate(char c);
/*Returns whether a letter, '#', or number is found and goes to that state*/
int idstate(char c);
/*Returns true if character value is a separator as defined in vector*/
bool isseparator(char c);
/*Returns true if character value is a operator as defined in vector*/
bool isoperator(char c);

#endif // !compiler_h
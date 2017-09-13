#pragma once
#ifndef compiler_h
#define compiler_h

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

//std::string keywords[] = {"while", "if", "int", "fi", "else", "return", "read", "write"};

void lexer(std::ifstream& input);
int getcase(char c);
std::string gettoken(std::ifstream& input);
std::string identifylexeme(std::string lexeme);
bool isseparator(char c);
bool isoperator(char c);

#endif // !compiler_h

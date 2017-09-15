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
int getstate(char c);
int numstate(char c);
int idstate(char c);
bool isseparator(char c);
bool isoperator(char c);

#endif // !compiler_h

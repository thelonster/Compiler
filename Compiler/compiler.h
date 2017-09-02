#ifndef compiler_h
#define compiler_h
#include <string>
#include <iostream>
#include <fstream>

std::string gettoken(std::ifstream input);
std::string getlexeme();
bool isseparator(char c);
bool isoperator(char c);
#endif // !compiler_h

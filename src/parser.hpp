#pragma once
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
using namespace std;

#include "executionScope.hpp"

class Parser 
{
private:
  vector<ExecutionScope> *executionScopes;
  Program convertToProgram(const string& plainText);

  /// MARK: Helpers
  vector<string> tokenizeLineOfCode(const string& lineOfCode);
  string removeLeadingWhitespace(const string& str);
public:
  Parser(vector<ExecutionScope>* executionScopes_);
  void parseFileContents(const string& contents);
};
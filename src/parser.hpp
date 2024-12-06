#pragma once
#include <vector>
#include <string>
using namespace std;

#include "executionScope.hpp"

class Parser 
{
private:
  vector<ExecutionScope> *executionScopes;
public:
  Parser(vector<ExecutionScope>* executionScopes_);
  void parseFileContents(string contents);
};
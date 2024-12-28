#pragma once
#include "executionScope.hpp"

// If it works don't fix it
class Executor;
// Because executor.hpp depends on executionScope.hpp that depends on expression.hpp
// that, if included, would make a cycle, so this is how the cycle is able to be stopped
// executor.hpp is included in the cpp so it can run

class Expression : public string
{
private:
  string removeParentheses(string str);
public:
  inline Expression(const string _expression): string(_expression)
  {}
  variableType evaluate(Executor *executor);
};

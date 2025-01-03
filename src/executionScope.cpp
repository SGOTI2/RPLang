#include "executionScope.hpp"

ExecutionScope::ExecutionScope(const program &instructions_)
  : instructions(instructions_)
{}

ExecutionScope::ExecutionScope(const program &instructions_, const string symbol_, const vector<rawToken> arguments_)
  : symbol(symbol_), instructions(instructions_), arguments(arguments_)
{}
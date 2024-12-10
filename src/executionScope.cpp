#include "executionScope.hpp"

ExecutionScope::ExecutionScope(const program &instructions_)
  : instructions(instructions_)
{}

ExecutionScope::ExecutionScope(const program &instructions_, const string symbol_)
  : symbol(symbol_), instructions(instructions_)
{}
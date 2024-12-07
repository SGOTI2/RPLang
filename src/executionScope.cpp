#include "executionScope.hpp"

ExecutionScope::ExecutionScope(const program &instructions_)
  : instructions(instructions_)
{}

ExecutionScope::ExecutionScope(const program &instructions_, const string symbol_)
  : symbol(symbol_), instructions(instructions_)
{}

void ExecutionScope::ExecuteScope()
{
  printf("Bonjour\n");
  printf("%s\n", instructions.at(0).first.c_str());
}

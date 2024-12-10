#include "builtin.hpp"

void rpl_builtin::log(ExecutionScope *scope, const vector<variableType> *args)
{
  vector<string> text = Parser::getAllArgsAs<string>(*args);
  for (const auto &arg : text)
    printf("%s", arg.c_str());

  printf("\n");
}

void rpl_builtin::run(Executor *executor, const vector<variableType> *args)
{
  string functionName = Parser::getArgAtAs<string>(*args, 0);
  for (ExecutionScope &scope : *executor->executionScopes)
  {
    if (scope.symbol == functionName) 
    {
      scope.halt = false;
      scope.instructionCounter = 0;
      scope.jmpReturnScope = executor->currentScope;
      executor->currentScope->halt = true;
      executor->currentScope = &scope;
      executor->execScope();
      return;
    }
  }
}

void rpl_builtin::return_(ExecutionScope *scope, const vector<variableType> *args)
{
  scope->halt = true;
}
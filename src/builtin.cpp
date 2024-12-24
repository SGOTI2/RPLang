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
      rpl_builtin::_runScope(executor, scope);
      return;
    }
  }
}

void rpl_builtin::_runScope(Executor *executor, ExecutionScope &scope)
{
  scope.halt = false;
  scope.instructionCounter = 0;
  scope.jmpReturnScope = executor->currentScope;
  executor->currentScope->halt = true;
  executor->currentScope = &scope;
  executor->execScope();
}

void rpl_builtin::return_(ExecutionScope *scope, const vector<variableType> *args)
{
  scope->halt = true;
}

void rpl_builtin::let(Executor *executor, const vector<variableType> *args) 
{
  if (args->size() != 2) 
    throw RplRuntimeError("Let requires exactly 2 arguments");
  executor->storeVariable(Parser::getArgAtAs<string>(*args, 0), args->at(1)); 
}

void rpl_builtin::_rpl_if(Executor *executor, const vector<variableType> *args)
{
  int scopeID = (int)Parser::getArgAtAs<double>(*args, 0);
  bool statement = Parser::getArgAtAs<bool>(*args, 1);

  executor->currentScope->previousIfStatementSuccess = statement;
  
  if (statement) 
  {
    ExecutionScope scope = executor->executionScopes->at(scopeID);
    rpl_builtin::_runScope(executor, scope);
  }
}

void rpl_builtin::_rpl_else(Executor *executor, const vector<variableType> *args)
{
  if (executor->currentScope->previousIfStatementSuccess)
    return;
  // We are going to run this else statement so don't let any future else statements run
  // Prevents double else condition ensuring only one else is to run
  executor->currentScope->previousIfStatementSuccess = true;

  int scopeID = (int)Parser::getArgAtAs<double>(*args, 0);
  
  ExecutionScope scope = executor->executionScopes->at(scopeID);
  rpl_builtin::_runScope(executor, scope);
}

void rpl_builtin::_rpl_elif(Executor *executor, const vector<variableType> *args)
{
  if (executor->currentScope->previousIfStatementSuccess)
    return;
    
  int scopeID = (int)Parser::getArgAtAs<double>(*args, 0);
  bool statement = Parser::getArgAtAs<bool>(*args, 1);

  executor->currentScope->previousIfStatementSuccess = statement;
  
  if (statement) 
  {
    ExecutionScope scope = executor->executionScopes->at(scopeID);
    rpl_builtin::_runScope(executor, scope);
  }
}
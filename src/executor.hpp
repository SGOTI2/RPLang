#pragma once
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

#include "executionScope.hpp"

class RplRuntimeError : public exception
{
private:
  const char *message;

public:
  RplRuntimeError(const char *msg) : message(msg) {}

  const char *what() const throw()
  {
    return message;
  }
};

class Executor; // Forward declaration of executor for use in the typedef's

typedef void (*executableCommand)(ExecutionScope *, const vector<variableType> *);
typedef void (*executorCommand)(Executor *, const vector<variableType> *);

class Executor
{
public:
  // Don't modify directly (for use in executor commands ONLY)
  vector<ExecutionScope> *executionScopes;
  ExecutionScope *currentScope = nullptr;
  unordered_map<string, executableCommand> commands;
  unordered_map<string, executorCommand> executorCommands;

  void execScope();

  // Public functions (safe to use)
  Executor(vector<ExecutionScope> *executionScopes_);
  void execRoot();

  // Load commands for execution
  void loadSymbols(unordered_map<string, executableCommand> &symbols);

  // Load commands that need the full executor context to run
  void loadExecutorSymbols(unordered_map<string, executorCommand> &execSymbols);
};

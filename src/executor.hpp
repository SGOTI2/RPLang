#pragma once
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

#include "executionScope.hpp"
#include "parser.hpp"

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
  unordered_map<string, variableType> variables;

  void execScope();

  void expandVariables(vector<variableType> *args);

  void _debugPrintVars();

  // Public functions (safe to use)
  Executor(vector<ExecutionScope> *executionScopes_);
  void execRoot();

  // Load commands for execution
  void loadSymbols(unordered_map<string, executableCommand> &symbols);

  // Load commands that need the full executor context to run
  void loadExecutorSymbols(unordered_map<string, executorCommand> &execSymbols);

  // Store/overwrite a variable in the program context 
  void storeVariable(const string &name, const variableType &value);

  // Load a variable
  variableType loadVariable(const string &name);
  variableType loadVariable(const string &name, const variableType &default_);
  template<typename T>
  T loadVariable(const string &name);
  template<typename T>
  T loadVariable(const string &name, const T &default_);
};

#include "executor.hpp"

void Executor::execScope()
{
  if (currentScope == nullptr)
    throw runtime_error("Current execution scope must not be nullptr");

  for(; currentScope->instructionCounter != currentScope->instructions.size(); currentScope->instructionCounter++)
  {
    if (currentScope->halt)
      break;

    const instruction currentInstruction = currentScope->instructions[currentScope->instructionCounter];

    string command = currentInstruction.first;
    
    auto execCmdIt = executorCommands.find(command);
    if (execCmdIt != executorCommands.end()) {
      executorCommand cmdFunction = execCmdIt->second;
      cmdFunction(this, &currentInstruction.second);

      continue;
    }

    auto cmdIt = commands.find(command);
    if (cmdIt == commands.end())
    {
      fprintf(stderr, "Command not found: %s\n", command.c_str());
      continue;
    }
    executableCommand cmdFunction = cmdIt->second;
    cmdFunction(currentScope, &currentInstruction.second);
  }
  if (currentScope->jmpReturnScope != nullptr)
  {
    currentScope = currentScope->jmpReturnScope;
    currentScope->halt = false;
  }
}

Executor::Executor(vector<ExecutionScope> *executionScopes_)
    : executionScopes(executionScopes_)
{
}

void Executor::execRoot()
{
  if (executionScopes->size() < 1)
    throw runtime_error("No root execution scope is present");
  currentScope = &executionScopes->at(0);
  execScope();
}

void Executor::loadSymbols(unordered_map<string, executableCommand> &symbols)
{
  commands.insert(symbols.begin(), symbols.end());
}

void Executor::loadExecutorSymbols(unordered_map<string, executorCommand> &execSymbols)
{
  executorCommands.insert(execSymbols.begin(), execSymbols.end());
}

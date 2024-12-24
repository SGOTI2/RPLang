#include "executor.hpp"

void Executor::execScope()
{
  if (currentScope == nullptr)
    throw RplRuntimeError("Current execution scope must not be nullptr");

  for (; currentScope->instructionCounter != currentScope->instructions.size(); currentScope->instructionCounter++)
  {
    if (currentScope->halt)
      break;

    instruction currentInstruction = currentScope->instructions[currentScope->instructionCounter];

    string command = currentInstruction.first;

    expandVariables(&(currentInstruction.second));

    auto execCmdIt = executorCommands.find(command);
    if (execCmdIt != executorCommands.end())
    {
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

void Executor::expandVariables(vector<variableType> *args)
{
  for (size_t i = 0; i < args->size(); i++)
  {
    visit([this, args, i](auto &&val) {
      using argT = decay_t<decltype(val)>;
      // Is of variable name type (string)
      if constexpr (is_same_v<string, argT>)
      {
        unordered_map<string, variableType>::iterator valueIt = variables.find(val);
        if (valueIt != variables.end())    
          args->at(i) = valueIt->second; // Assign the value of the var
      } 
    }, args->at(i));
  }
}

void Executor::_debugPrintVars()
{
  for (auto &var : variables)
  {
    string vStr = "Indescribable Value";
    variableType varValue = var.second;
    if (holds_alternative<string>(varValue))
      vStr = get<string>(varValue);
    else if (holds_alternative<double>(var.second))
      vStr = to_string(get<double>(varValue)) + "f";
    else if (holds_alternative<bool>(var.second))
      vStr = get<bool>(varValue) ? "true" : "false";

    printf("%s: %s\n", var.first.c_str(), vStr.c_str());
  }
}

Executor::Executor(vector<ExecutionScope> *executionScopes_)
    : executionScopes(executionScopes_)
{
}

void Executor::execRoot()
{
  if (executionScopes->size() < 1)
    throw RplRuntimeError("No root execution scope is present");
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

void Executor::storeVariable(const string &name, const variableType &value)
{
  variables.emplace(name, value);
}

variableType Executor::loadVariable(const string &name)
{
  auto value = variables.find(name);
  if (value == variables.end())
    throw RplRuntimeError("Variable was not found");
  return value->second;
}

variableType Executor::loadVariable(const string &name, const variableType &default_)
{
  auto value = variables.find(name);
  if (value == variables.end())
    return default_;
  return value->second;
}

template <typename T>
T Executor::loadVariable(const string &name)
{
  auto value = variables.find(name);
  if (value == variables.end())
    throw RplRuntimeError("Variable was not found");
  return Parser::getAs<T>(value->second);
}

template <typename T>
T Executor::loadVariable(const string &name, const T &default_)
{
  auto value = variables.find(name);
  if (value == variables.end())
    return default_;
  return Parser::getAs<T>(value->second);
}
#pragma once
#include <vector>
#include <variant>
#include <unordered_map>
using namespace std;

typedef variant<
  string, 
  double, 
  bool 
> variableType;

typedef pair<string, vector<variableType>> instruction;
/*
  [
    (string CMD_Name, [VariableType args...]),
    (string CMD_Name, [VariableType args...]),
    (string CMD_Name, [VariableType args...]),
    (string CMD_Name, [VariableType args...])
    ...
  ]
*/
typedef vector<instruction> program;

class ExecutionScope
{
public:
  size_t instructionCounter = 0;
  ExecutionScope *jmpReturnScope = nullptr;
  bool halt = false;
  unordered_map<string, variableType> internal_variables;
  string symbol = "";
  program instructions;

  ExecutionScope(const program &instructions_);
  ExecutionScope(const program &instructions_, const string symbol_);
};
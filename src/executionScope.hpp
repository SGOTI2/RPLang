#pragma once
#include <vector>
#include <variant>
#include <unordered_map>
using namespace std;

class rawToken : public string
{
public:
  rawToken(const string _str): string(_str) 
  {};
};

static_assert(!is_same_v<rawToken, string>, "rawToken cannot be the same type as string");

class Expression;

typedef variant<
  rawToken,
  Expression,
  string, 
  double, 
  bool 
> variableType;

#include "expression.hpp"

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
  string symbol = "";
  program instructions;
  bool previousIfStatementSuccess = false;

  ExecutionScope(const program &instructions_);
  ExecutionScope(const program &instructions_, const string symbol_);
};
#pragma once
#include <vector>
#include <variant>
#include <unordered_map>
using namespace std;

typedef variant<
  string, 
  double, 
  bool 
> VariableType;

/*
  [
    (string CMD_Name, [VariableType args...]),
    (string CMD_Name, [VariableType args...]),
    (string CMD_Name, [VariableType args...]),
    (string CMD_Name, [VariableType args...])
    ...
  ]
*/
typedef vector<
  pair<string, vector<VariableType>>
> Program;

class ExecutionScope 
{
private:
  string symbol = "";
  Program program;
  int instructionCounter = 0;
  ExecutionScope* jmpReturnScope = nullptr;
  bool halt = false;
  unordered_map<string, VariableType> internal_variables;

public:
  ExecutionScope();
  void ExecuteScope();
};
#pragma once
#include "executor.hpp"
#include "parser.hpp"

namespace rpl_builtin
{
  void log(ExecutionScope* scope, const vector<variableType>* args);
  void run(Executor* executor, const vector<variableType>* args);
  void _runScope(Executor* executor, ExecutionScope& scope);
  void return_(ExecutionScope* scope, const vector<variableType>* args);
  void let(Executor* executor, const vector<variableType>* args);
  void _rpl_if(Executor* executor, const vector<variableType>* args);
  void _rpl_else(Executor* executor, const vector<variableType>* args);
  void _rpl_elif(Executor* executor, const vector<variableType>* args);

  static inline unordered_map<string, executableCommand> libSymbols {
    {"log", log},
    {"return", return_},
  };

  static inline unordered_map<string, executorCommand> execSymbols {
    {"run", run},
    {"let", let},
    {"_rpl_if", _rpl_if},
    {"_rpl_else", _rpl_else},
    {"_rpl_elif", _rpl_elif}
  };
}
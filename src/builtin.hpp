#pragma once
#include "executor.hpp"
#include "parser.hpp"

namespace rpl_builtin
{
  void log(ExecutionScope* scope, const vector<variableType>* args);
  void run(Executor* executor, const vector<variableType>* args);
  void return_(ExecutionScope* scope, const vector<variableType>* args);


  static inline unordered_map<string, executableCommand> libSymbols {
    {"log", log},
    {"return", return_}
  };

  static inline unordered_map<string, executorCommand> execSymbols {
    {"run", run}
  };
}
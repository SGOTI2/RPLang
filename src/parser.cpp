#include "parser.hpp"

Parser::Parser(vector<ExecutionScope> *executionScopes_)
: executionScopes(executionScopes_)
{
  executionScopes->clear();
}

void Parser::parseFileContents(string contents)
{
  executionScopes->push_back(ExecutionScope());
  if (executionScopes->size() < 1)
  {
    fprintf(stderr, "Parsing Error: No root context was established\n");
    exit(EXIT_FAILURE);
  }
}
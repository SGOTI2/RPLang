#pragma once
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
using namespace std;

#include "executionScope.hpp"

class ParsingError : public exception
{
private:
  const char *message;

public:
  ParsingError(const char *msg) : message(msg) {}

  const char *what() const throw()
  {
    return message;
  }
};

class Parser
{
private:
  vector<ExecutionScope> *executionScopes;
  program convertToProgram(const string &plainText);
  // Detect and remove sub-scopes from the instructions for a given scope, return where the scope ends so it can be removed
  size_t detectSubScopes(program *instructions);

  /// MARK: Helpers
  vector<string> tokenizeLineOfCode(const string &lineOfCode);
  string removeLeadingWhitespace(const string &str);
  variableType tokenToType(const string &token);

public:
  Parser(vector<ExecutionScope> *executionScopes_);
  void parseFileContents(const string &contents);

  template <typename T>
  static T getArgAtAs(const vector<variableType> &args, size_t index);

  template <typename T>
  static vector<T> getAllArgsAs(const vector<variableType> &args);
};

template <typename T>
inline T Parser::getArgAtAs(const vector<variableType> &args, size_t index)
{
  if (args.size() < index)
  {
    throw ParsingError("Too few arguments");
  }
  variableType arg = args.at(index);

  if (holds_alternative<T>(arg))
  {
    return get<T>(arg);
  }
  throw ParsingError("Argument is a incorrect type");
}

template <typename T>
inline vector<T> Parser::getAllArgsAs(const vector<variableType> &args)
{
  vector<T> result;
  for (auto &v : args)
  {
    visit([&result](auto &&val) 
    {
      using argT = decay_t<decltype(val)>;
      if constexpr (is_same_v<T, argT>)
      {
        result.push_back(val);
      }
    }, v);
  }
  return result;
}

#include "expression.hpp"
#include "executor.hpp"
#include "parser.hpp"

string Expression::removeParentheses(string str)
{
  size_t strBegin = str.find_first_of("(");
  if (strBegin == string::npos)
    strBegin = -1;

  size_t strEnd = str.find_last_of(")");
  if (strEnd == string::npos)
    strEnd = str.size();
  
  const size_t strRange = strEnd - strBegin - 1;
  
  return str.substr(strBegin + 1, strRange);
}

variableType Expression::evaluate(Executor *executor)
{
  string exprString = removeParentheses(string(*this));

  vector<string> tokens = Parser::tokenizeLineOfCode(exprString);

  vector<variableType> typedTokens = Parser::LOCTokensToType(tokens);

  size_t numberOfTokens = typedTokens.size();
  
  if (numberOfTokens == 0)
    throw ParsingError("Empty parentheses are not allowed");
  if (numberOfTokens == 1)
    return typedTokens.at(0);
  if (numberOfTokens != 3)
    throw ParsingError("Invalid expression, expressions must be exactly 1 or 3 arguments");
  
  executor->expandVariables(&typedTokens);
  executor->expandExpressions(&typedTokens); // Recursion in here, allows for ((true || false) && true) to work

  variableType lhs = typedTokens.at(0);
  rawToken op = Parser::getAs<rawToken>(typedTokens.at(1));
  variableType rhs = typedTokens.at(2);

  if (holds_alternative<rawToken>(lhs) || holds_alternative<rawToken>(rhs))
    throw ParsingError("Cannot evaluate expression with a non-resolved variable");

  if (op == "==")
    return lhs == rhs;
  if (op == "!=")
    return lhs != rhs;
  if (op == "&&")
    return Parser::getAs<bool>(lhs) && Parser::getAs<bool>(rhs);
  if (op == "||")
    return Parser::getAs<bool>(lhs) || Parser::getAs<bool>(rhs);
  if (op == "+")
    return Parser::getAs<double>(lhs) + Parser::getAs<double>(rhs);
  if (op == "-")
    return Parser::getAs<double>(lhs) - Parser::getAs<double>(rhs);
  if (op == "/") {
    double divisor = Parser::getAs<double>(rhs);
    if (divisor == 0)
      throw ParsingError("Division by 0");
    return Parser::getAs<double>(lhs) / divisor;
  }
  if (op == "*")
    return Parser::getAs<double>(lhs) * Parser::getAs<double>(rhs);


  throw ParsingError("Illegal expression operator");
}

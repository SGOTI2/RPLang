#include "parser.hpp"

Parser::Parser(vector<ExecutionScope> *executionScopes_)
    : executionScopes(executionScopes_)
{
  executionScopes->clear();
}

void Parser::parseFileContents(const string &contents)
{
  // Convert the file into a program, this itself does not establish contexts in the program
  program fullProgram = convertToProgram(contents);

  (void)detectSubScopes(&fullProgram);

  executionScopes->insert(executionScopes->begin(), ExecutionScope(fullProgram));
}

program Parser::convertToProgram(const string &plainText)
{
  program resultingProgram;

  stringstream plainTextStream(plainText);
  string line;
  char lineSplitter = ';';

  while (getline(plainTextStream, line, lineSplitter))
  {
    vector<string> tokens = tokenizeLineOfCode(line);

    if (tokens.size() < 1)
      continue;

    // Get the command (it's the first token) and remove it from the tokens
    string command = tokens.front();
    tokens.erase(tokens.begin());

    vector<variableType> args = LOCTokensToType(tokens);

    instruction thisInstruction = make_pair(command, args);
    resultingProgram.push_back(thisInstruction);
  }

  return resultingProgram;
}

size_t Parser::detectSubScopes(program *instructions, bool subscopeStopsAtElse)
{
  /* Steps for detecting sub-scopes
    1. Look at the command for each instruction in the program
    2. If the command is one that starts a scope (func, if, elif, etc.), create a new scope and run this function again to find it's sub-scopes
      a. Take where the scope ended and remove from where the scope started and ended (returned by this function) to isolate the code for this scope only
    3. If the command is one that ends a scope (end), remove the rest of the instructions for this scope and return where this scope ended
  */
  program::iterator it = instructions->begin();
  while (it != instructions->end())
  {
    // Switch command name
    string cmdName = (*it).first;

    bool elseIsEndingScope = (subscopeStopsAtElse && (cmdName == "else" || cmdName == "elif"));

    // Subscope End Tokens
    if (cmdName == "end" || elseIsEndingScope)
    {
      // Remove the following instructions, 
      instructions->erase(it, instructions->end());

      if (elseIsEndingScope)
        return instructions->size() + 1; // Same as below but don't remove the else part because it needs to start the next scope

      // Return size of the scope +2 so the end line is included + line for scope start
      return instructions->size() + 2;
    }
    else if (cmdName == "func") 
    {
      string symbol = getArgAtAs<string>((*it).second, 0);

      program subProgram(it + 1, instructions->end());
      size_t scopeLength = detectSubScopes(&subProgram);
      executionScopes->push_back(ExecutionScope(subProgram, symbol));

      it = instructions->erase(it, it + scopeLength);
      continue;
    }
    else if (cmdName == "if" || cmdName == "else" || cmdName == "elif")
    {
      // Get the _rpl_if args ready while `it` is still at the right spot
      vector<variableType> args = {(double)executionScopes->size() + 1};
      args.insert(args.end(), (*it).second.begin(), (*it).second.end());

      // Discover and then create the scope for the if statement
      program subProgram(it + 1, instructions->end());
      size_t scopeLength = detectSubScopes(&subProgram, true); // Subscope should end at else / elif
      executionScopes->push_back(ExecutionScope(subProgram));

      if (scopeLength == 0)
        throw ParsingError("If statement must contain some executable code");

      // Remove code in the if's scope from the main scope
      it = instructions->erase(it, it + scopeLength);

      if (cmdName == "if")
        instructions->insert(it, {"_rpl_if", args});
      else if (cmdName == "elif")
        instructions->insert(it, {"_rpl_elif", args});
      else if (cmdName == "else") {
        if (args.size() > 2) {
          printf("Parsing: Else was converted to elif because it was passed an argument");
          instructions->insert(it, {"_rpl_elif", args});
          continue;
        }

        instructions->insert(it, {"_rpl_else", args});
      }
      continue;
    }
    ++it;
  }
  return 0;
}

/// MARK: Helpers

vector<variableType> Parser::LOCTokensToType(const vector<string> &tokens)
{
  // Generally all of the tokens except for strings and expressions which are combined into 1 arg
  vector<variableType> args = {};
  bool awaitingStringClose = false;
  string stringVar = "";

  int expressionDepth = 0;
  string currentExpression = "";

  for (const string &token : tokens)
  {
    // Don't make expressions in strings!
    if (!awaitingStringClose) 
    {
      expressionDepth += count(token.begin(), token.end(), '(');
      if (expressionDepth > 0) {
        if (currentExpression != "")
          currentExpression += " ";
        currentExpression += token;
      }
      expressionDepth -= count(token.begin(), token.end(), ')');

      if (currentExpression != "" && expressionDepth <= 0) 
      {
        if (expressionDepth < 0)
          throw ParsingError("Expression has too many closing parentheses");
        args.push_back(Expression(currentExpression));
        currentExpression = "";
        continue;
      }

      // No token parsing in Expressions they will handle themselves, this includes strings.
      // See the Expression::evaluate function for the full implementation
      if (expressionDepth > 0)
        continue;
    }


    // Token begins a quote
    if (token.front() == '\"' && token != "\"")
    {
      awaitingStringClose = true;
      stringVar = "";
    }

    // We are in a quote
    if (awaitingStringClose) 
    {
      if (stringVar != "")
        stringVar += " ";
      
      stringVar += token;
    } 
    else
      args.push_back(tokenToType(token));

    // Token ends a quote
    if (token.back() == '\"')
    {
      awaitingStringClose = false;
      // Put string arg in args, removing the leading and trailing quotes
      args.push_back(stringVar.substr(1, stringVar.size() - 2));
    }
  }

  if (awaitingStringClose)
    throw ParsingError("String not closed");
  
  return args;
}

vector<string> Parser::tokenizeLineOfCode(const string &lineOfCode)
{
  string fixedLineOfCode = removeLeadingWhitespace(lineOfCode);

  vector<string> tokens;
  istringstream stream(fixedLineOfCode);
  string word;

  // Extract words separated by whitespace
  while (stream >> word)
  {
    tokens.push_back(word);
  }

  return tokens;
}

string Parser::removeLeadingWhitespace(const string &str)
{
  // Find the first non-whitespace character
  auto start = find_if(str.begin(), str.end(), [](unsigned char ch)
                       { return !isspace(ch); });

  // Return the substring from the first non-space character to the end
  return string(start, str.end());
}

/// TODO: Implement type conversions, ex.: "1.2" -> (double)1.2, "52" -> (int)52, "true" -> (bool)true
variableType Parser::tokenToType(const string &token)
{
  if (token == "true")
    return true;
  else if (token == "false")
    return false;
  char *endCharPtr;
  double vDouble = strtod(token.c_str(), &endCharPtr);
  // End char is null, no end char, entire string is a double
  if (!*endCharPtr || *endCharPtr == 'f')
    return vDouble;
  return rawToken(token);
}

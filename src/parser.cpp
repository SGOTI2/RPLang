#include "parser.hpp"
// Iterator is shortened to It in this program

Parser::Parser(vector<ExecutionScope> *executionScopes_)
    : executionScopes(executionScopes_)
{
  executionScopes->clear();
}

void Parser::parseFileContents(const string& contents)
{
  // Convert the file into a program, this does not establish contexts in the program
  Program fullProgram = convertToProgram(contents);

  executionScopes->push_back(ExecutionScope());
  if (executionScopes->size() < 1)
  {
    throw length_error("No main execution scope was created");
  }
}

Program Parser::convertToProgram(const string& plainText)
{
  Program program;
  
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

    bool awaitingStringClose = false;
    for(const string& token : tokens) {

    }

  }
}

/// MARK: Helpers

vector<string> Parser::tokenizeLineOfCode(const string &lineOfCode)
{
  string fixedLineOfCode = removeLeadingWhitespace(lineOfCode);

  vector<string> tokens;
  istringstream stream(fixedLineOfCode);
  string word;
  
  // Extract words separated by whitespace
  while (stream >> word) {
    tokens.push_back(word);
  }
  
  return tokens;
}

string Parser::removeLeadingWhitespace(const string &str)
{
  // Find the first non-whitespace character
  auto start = find_if(str.begin(), str.end(), [](unsigned char ch) {
    return !isspace(ch);
  });

  // Return the substring from the first non-space character to the end
  return string(start, str.end());
}

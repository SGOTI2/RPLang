// This is to demonstrate the RPL language

#include <fstream>
#include <string>
using namespace std;

#include "parser.hpp"
#include "executionScope.hpp"

string ReadFile(string fileName)
{
  string content;
  ifstream programFile {fileName};
  if (programFile.is_open())
  {
    // Read the entire contents of the file
    content.assign( (std::istreambuf_iterator<char>(programFile) ),
                    (std::istreambuf_iterator<char>()    ) );
  }

  else 
  {
    throw runtime_error("The program was not able to be read");
  }

  return content;
}

int main() 
{
  vector<ExecutionScope> executionScopes;
  Parser parser {&executionScopes};

  string programFileContents = ReadFile("program.rpl"); // I'm well aware of how bad hard coding a path like this is, this file shouldn't even be in the library anyway

  parser.parseFileContents(programFileContents);

  executionScopes.at(0).ExecuteScope();

  return 0;
}
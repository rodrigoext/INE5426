#include <iostream>
#include "ast.h"

extern AST::Block* programRoot; //set on Bison files
extern int yyparse();

int main(int argc, char **argv)
{
 yyparse();                  //parses whole data
 std::cout << "#Print lines" << std::endl;
 programRoot->printTree();   //prints the ASTs
 std::cout << "#Compute lines" << std::endl;
 programRoot->computeTree(); //computes the ASTs
 return 0;
}

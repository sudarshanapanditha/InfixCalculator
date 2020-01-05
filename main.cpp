#include <cstdio>
#include <exception>
#include "token.hpp"
#include "stack.hpp"
#include "funcs.hpp"



int main()
{
  int i, j;
  int count;
  FILE *fp;
  Token inFix[MAX_EXPRESSION_SIZE], postFix[MAX_EXPRESSION_SIZE];
  int esiz, psiz;
  printf("Input expression\n");

  fp = stdin;
  readExpression(fp, inFix, &esiz);

  convertToPostFix(inFix, postFix, esiz, &psiz);

  // evaluating postFix
  double output;

  // Evaluating the converted postFix
  try{
    output = evaluatePostFix(postFix, psiz);
    printf("output: %lf\n", output);
  }catch(std::exception & ex)
  {
    printf("%s\n", ex.what());
  }

  for (i=0;i<psiz;i++) {
    printf("TOKEN %d : ", i+1);
    switch(postFix[i].type) {
    case LBRACE:
      printf("Lbrace ");
      break;
    case RBRACE:
      printf("Rbrace ");
      break;
    case OPERATOR:
      printf("operator:%s ", postFix[i].value.cval);
      break;
    case VALUE:
      printf("operand:%1.1f ", postFix[i].value.fval);
      break;
    }
    printf("\n");
  }
  return 0;
}

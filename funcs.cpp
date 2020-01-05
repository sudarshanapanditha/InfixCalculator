#include <cstdio>
#include <cstring>
#include <cmath> // to compute squareroot
#include <exception> // for error reporting
#include "token.hpp"
#include "stack.hpp"
#include "funcs.hpp"

// exception class for error handling
class PostFixCalculatorException: public std::exception
{
private:
  const char* msg = nullptr;
  PostFixCalculatorException(){}
public:
  PostFixCalculatorException(const char* s) throw():msg(s){}
  const char * what() const throw(){ return msg;};
};

void calculate(Token *oper, double * output, double * arg1, double * arg2 = nullptr)
{
  const char *opstr = oper->value.cval;

  // arg1 is a nullptr
  if( arg1 == nullptr)
    throw PostFixCalculatorException("Error: No operands to operate on.");

  if(!strcmp(opstr, "sqrt"))
  {
    *output = std::sqrt(*arg1);
  }else if(!strcmp(opstr, "+")){
    *output = *arg1 + *arg2;
  }else if(!strcmp(opstr, "-")){
    *output = *arg1 - *arg2;
  }else if(!strcmp(opstr, "*")){
    *output = *arg1 * *arg2;
  }else if(!strcmp(opstr, "/")){
    if(*arg2 == 0)
      throw PostFixCalculatorException("Error: Division by zero.");
    *output = *arg1 / *arg2;
  }
}

int isBinaryOperator(Token *oper)
{
  const char* opstr = oper->value.cval;
  if(!strcmp(opstr, "sqrt")) return 0; // squareroot is a unary operator
  return 1;
}

int hasBalancedParanthesis(Token *expr, int exprsiz)
{
  int i;
  stack s;
  Token* token;
  for(i = 0; i < exprsiz ; ++i)
  {
    if(expr[i].type == LBRACE)
    {
      s.Push(&expr[i]);
    }else if(expr[i].type == RBRACE)
    {
      // empty stack, there is no matching LBRACE
      if(s.isEmpty()) return 0;
      // stack is not empty
      token = s.Pop();
      if(token->type != LBRACE) return 0;
    }
  }
  // if stack is empty, it is balanced
  return s.isEmpty();
}

double evaluatePostFix(Token *postFix, int psiz)
{
  int i;
  double arg1, arg2, tempout;
  Token *token;
  stack s;

  // check for balanced paranthesis
  if(hasBalancedParanthesis(postFix, psiz))
  {
    for(i = 0; i < psiz; ++i)
    {
      switch(postFix[i].type)
      {
        case VALUE:
          s.Push(&postFix[i]);
          break;

        case OPERATOR:
          // stack is empty, there should be atleast one operand
          if(s.isEmpty())
            throw PostFixCalculatorException("Error: Insufficient operands.");

          // check if the operator is binary
          if(isBinaryOperator(&postFix[i]))
          {
            // pop second operand
            token = s.Pop();
            arg2 = token->value.fval;
            delete token; // freeing memory of token
            // stack is empty, need atleast two operands
            if(s.isEmpty())
              throw PostFixCalculatorException("Error: Insufficient operands. Two operands are needed.");
            token = s.Pop();
            arg1 = token->value.fval;
            delete token; // freeing memory of token
            // calculate
            calculate(&postFix[i], &tempout, &arg1, &arg2);
          }else{
            token = s.Pop();
            arg1 = token->value.fval;
            delete token;
            // calculate
            calculate(&postFix[i], &tempout, &arg1);
          }
          // creating new Token with intermediate result
          token = new Token();
          token->type = VALUE;
          token->value.fval = tempout;
          s.Push(token);
          break;
        default: break;
      }
    }
  }else{
    // error, unbalanced paranthesis
    throw PostFixCalculatorException("Error: Unbalanced Paranthesis.");
  }
  token = s.Pop();
  // error: stack is not empty, some Tokens are not evaluated
  if(!s.isEmpty())
    throw PostFixCalculatorException("Error: Invalid Expression.");
  // no errors, can set the output
  tempout = token->value.fval;
  delete token; // freeing memory of token
  return tempout;
}

void convertToPostFix(Token *inFix, Token *postFix, int insiz, int *psizp)
{
  stack s;
  int psiz;
  int i, done;
  Token *token;


  psiz = 0;

  for (i=0;i<insiz;i++) {

    switch(inFix[i].type) {

    case VALUE:
      postFix[psiz] . type = inFix[i].type;
      postFix[psiz++]. value.fval = inFix[i].value.fval;
      break;

    case LBRACE:
      s.Push(&inFix[i]);
      break;

    case RBRACE:

      token = s.Pop();

      while(token->type != LBRACE) {
	postFix[psiz] . type = token->type;
	strcpy(postFix[psiz++]. value.cval, token->value.cval);
	token = s.Pop();
      }

      break;

    case OPERATOR:
      done = 0;
      while (!s.isEmpty()  && !done) {

	token = s.Pop();

	if (token->type != LBRACE  && precedence(token->value.cval) >=
	    precedence(inFix[i].value.cval)) {

	  postFix[psiz] . type = token->type;
	  strcpy(postFix[psiz++]. value.cval, token->value.cval);
	}

	else
	  {
	    s.Push(token);
	    done = 1;
	  }

      }
      s.Push(&inFix[i]);

      break;
    }
  }


  while (!s.isEmpty()) {
    token = s.Pop();
    postFix[psiz].type = token->type;
	strcpy(postFix[psiz++].value.cval, token->value.cval);
  }

  *psizp = psiz;
}








void readExpression(FILE *fp, Token *expression, int *esizp)
{
  char ch;
  char expr[MAX_EXPRESSION_SIZE], op[100], *p;
  float f;
  int i;
  int esiz;

  ch = getc(fp);
  esiz = 0;
  while (!endOfLine(ch)) {

    if (isSpace(ch)){
    }
    else

      if (isNumeral(ch)) {
	ungetc(ch, fp);
	fscanf(fp, "%f", &f);

	expression[esiz].type = VALUE;
	expression[esiz++].value.fval = f;


      } else

	if (isRightBrace(ch)) {
	  expression[esiz++].type = RBRACE;
	}

	else
	  if (isLeftBrace(ch))
	    expression[esiz++].type = LBRACE;
	  else
	    {
	      ungetc(ch, fp);
	      readOperator(fp, op);
	      expression[esiz].type = OPERATOR;
	      strcpy(expression[esiz++].value.cval, op);
	    }
    ch = getc(fp);
  }
  *esizp = esiz;
}

int isSpace(char ch)
{
  return (ch==' ');
}

int isLeftBrace(char ch)
{
  return (ch == '(');
}

int isRightBrace(char ch)
{
  return (ch == ')');
}

int isNumeral(char ch)
{
  return (ch >= '0' && ch <= '9');
}

void readOperator(FILE *fp, char *op)
{
  int i;
  int j = 0;
  char ch;

  j = 0;
  ch = getc(fp);
  while(!isRightBrace(ch) && !isLeftBrace(ch) && !isNumeral(ch)
	&& !endOfLine(ch)) {
    op[j++] = ch;

    op[j] = '\0';

    if (isValidOperator(op))
      break;

    ch = getc(fp);
  }
}

int isValidOperator(char *op)
{
  int i;

  for (i=0;i<NUM_OPS;i++)
    if (!strcmp(op, OPS[i]))
      break;

  if (i<NUM_OPS)
    return 1;
  return 0;
}

int precedence(char *op)
{
  int i;

  if (!strcmp(op, "sqrt"))
    return 3;
  if (!strcmp(op, "*") || !strcmp(op, "/"))
    return 2;
  if (!strcmp(op, "+") || !strcmp(op, "-"))
    return 1;

  return -1;
}




int endOfLine(char ch)
{
  return (ch == '\n');
}

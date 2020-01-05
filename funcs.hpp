int isNumeral(char ch);
int isRightBrace(char ch);
int isLeftBrace(char ch);
int isSpace(char ch);
int endOfLine(char ch);
void readExpression(FILE *fp, Token *expression, int *esizp);
void convertToPostFix(Token *inFix, Token *postFix, int insiz, int *psizp);
void readOperator(FILE *fp, char *op);
int isValidOperator(char *op);
int precedence(char *op);
/* evaluating a postfix expression */
int hasBalancedParanthesis(Token *expr, int exprsiz);
int isBinaryOperator(Token *oper);
void calculate(Token *oper, double * output, double * arg1, double * arg2);
double evaluatePostFix(Token *postFix, int psiz);
// class for error handling
class PostFixCalculatorException;

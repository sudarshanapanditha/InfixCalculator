#define NUM_OPS 5
#define OP_SIZE 10
#define MAX_EXPRESSION_SIZE 100

typedef enum {LBRACE, RBRACE, VALUE, OPERATOR} TokenType;

struct Token {
  TokenType type;
  union {
    float fval;
    char cval[OP_SIZE];
  } value;
};

extern char OPS[NUM_OPS][OP_SIZE];



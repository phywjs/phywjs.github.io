#include "defines.h"

/* The interactive calculator simulates a hand-held
   calculator.  The calculator evaluates expressions
   until the user signals halt.  An expression
   consists of a left operand, an operator, and a right
   operand.  The left operand is initialized to zero
   and is automatically displayed as part of the 
   prompt for user input.  The user inputs the right
   operand, an expression that consists of constants
   and operators.  To halt, the user enters the stop
   character '!' as the first operator in the right operand.
*/

int lhs = 0;    /* Left-hand side */
char buffer[MaxExp+2];   /* input/putput buffer */
static char* ops = "+-*/%!";

void prompt(void),
     input (void),
     output (void), 
     update_lhs(void);
int bad_op(char);

main()
{
   while(Forever) {
      prompt();
      input();
      output();
   }
}


int bad_op(char c)
{
   return NULL == strchr(ops, c);
}

/* Parse the right operand, and expresion that 
   consists of binary operators and integer 
   constants.  In case of error, do not update
   left operand.  Otherwise, set left operand
   to current value of calculation.
*/
void update_lhs(void)
{
   char *next = buffer, *last;
   char op[10], numStr[20];
   int num;
   int temp = lhs;

   last = next + strlen(buffer);
   while (next < last) {
      if ( sscanf(next, "%s %s", op, numStr) < 2 ||
           strlen(op) != 1 ||
           bad_op(op[0]) )
         return;
      if (Stop == op[0] ) {
         printf(" *** End of Calculation ***\n\n");
         exit(EXIT_SUCCESS);
      }
      num = atoi(numStr);

      switch(op[0]) {
         case '+': temp += num; break;
         case '-': temp -= num; break;
         case '*': temp *= num; break;
         case '/': temp /= num; break;
         case '%': temp %= num; break;
      }

      next += strlen(op) + strlen(numStr) + 2;
   }
   lhs = temp;
}



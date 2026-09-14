/*  This program computes the resistance in ohms for a resistor.
    It prints a menu of colors of bands that mark a resistor and asks
    the user to enter character codes for each band; for example,
    the user might eneter 'B' for black.   The program decodes the
    characters, computes the resistance, and prints the result.

    The function main invokes print_codes to print the codes,
    getchar to read three such codes from the keyboard, and decode_char
    to do the decoding.  The function main uses a standard library
    function, pow, which raises a number to a power to help
    compute the resistance; main then prints the resistance as
    a floating-point number.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print_codes(void);
double decode_char(char code);

main()
{
   char code1, code2, code3;
   double R;
   double c1, c2, c3;
   int flag;

   print_codes();
   printf("\n\nEnter three codes. ");
   code1 = getchar();
   code2 = getchar();
   code3 = getchar();
  
   c1 = decode_char(code1);
   c2 = decode_char(code2);
   c3 = decode_char(code3);

   if(c1 == -999.0 || c2 == -999.0 || c3 == -999.0)
      printf("\n\n\tBad code - cannot compute resistnace\n");
   else {
      R = (10.0*c1 + c2)*pow(10.0, c3);
      printf("\n\n\tResistance in ohms:\t%f\n", R);
   }
   return EXIT_SUCCESS;
}

/*  This function prints a menus of color codes to guide the
    user in entering input. */
void print_codes(void)
{
   printf("\n\n\tThe colored bands are coded as follows:\n\n\t");
   printf("COLOR\t\t\tCODE\n\t");
   printf("-----\t\t\t----\n\n");
   printf("\tBlack--------> B\n");
   printf("\tBrown--------> N\n");
   printf("\tRed----------> R\n");
   printf("\tOrange-------> O\n");
   printf("\tYellow-------> Y\n");
   printf("\tGreen--------> G\n");
   printf("\tBlue---------> E\n");
   printf("\tViolet-------> V\n");
   printf("\tGray---------> A\n");
   printf("\tWhite--------> W\n");
}


/* This function expects a character (color code) and returns
   a double precision floating-point number as its value.  If the
   code is not legal, it returns a value that signals that fact. */
double decode_char(char code)
{
   switch (code) {
      case 'B':
         return 0.0;
      case 'N':
         return 1.0;
      case 'R':
         return 2.0;
      case 'O':
         return 3.0;
      case 'Y':
         return 4.0;
      case 'G':
         return 5.0;
      case 'E':
         return 6.0;
      case 'V':
         return 7.0;
      case 'A':
         return 8.0;
      case 'W':
         return 9.0;
      default:
         return -999.0;
   }
}

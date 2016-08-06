
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <ctype.h>

/*
   determining whether a character is a vowel
*/


int isvowel (char cc)
{
  static char vowel[] = { 'a', 'e', 'i', 'o', 'u' };
  static int nvowels = sizeof vowel / sizeof *vowel;
  int ii;
  
  cc = tolower (cc);
  for (ii = 0; ii < nvowels; ++ii) {
    if (cc == vowel[ii]) {
      return 1;
    }
  }
  
  return 0;
}
//================================
struct mode {
int arry;
char def[3];
};
//====================================
//========= finding the maximum

struct mode find_max(int x, int y, int z){
 struct mode temp ;
 temp.def[0] = ' ';
 temp.def[1] =  ' ';
 temp.def[2] = ' ';
 int max = x;
  if (max< y)
    max = y;
 if (max<z)
    max=z;

if (x == max) temp.def[0] = 'm';
if (y == max) temp.def[1] = 'i';
if (z == max) temp.def[2] = 'd';
 temp.arry = max; 
return temp;
}
//========================================
//======== Convert uppercase to lowercase
void upp_low(char *str){
int i;
for (i=1;i<=strlen(str);i++)
if (str[i] >=65 && str[i] <=90)
 str[i]= str[i]+32;
return ;
}

//===============================================
//=========== trace back function ===============

int traceback(int m,int n,struct mode **matrix,char char_array[5][100]){
m--;
n--;
int ii=1;
while ( (m+1)>0 && (n)>0 )
{
 if (matrix[m][n].def[0] == 'm') 
   {char_array[4][ii] = char_array[1][m];char_array[5][ii] = char_array[2][n]; m--;n--;}
  else
  if (matrix[m][n].def[1] == 'i')
   {char_array[4][ii] = '_'; char_array[5][ii] = char_array[2][n];n--;}
   else
     if (matrix[m][n].def[2] == 'd')
      {char_array[4][ii] = char_array[1][m]; char_array[5][ii] = '_' ;m--;}
ii++;
}
return --ii;

}





//====================================
//
//            main
//
//====================================
int main (int argc, char ** argv)
{
 
 char *chk, char_array[5][100];
  int ii;
  
  /*
    Check that we were given two arguments.
  */
  if (argc < 3) {
    errx (EXIT_FAILURE, "please provide two strings on the command line");
  }
  
  /*
    Check that each argument contains letters only and save the dimension of the matrix.
  */
int def_matrix_dimension[3], iii; 
char_array[1][0]='0';
char_array[2][0]='0';
 for (ii = 1; ii < 3; ++ii) {
    for (iii=1 , chk = argv[ii]; *chk != '\0'; ++chk , iii++) {
       if (0 == isalpha (*chk)) {
	errx (EXIT_FAILURE, "invalid character code %d in argument %d", (int) *chk, ii);
      }
    char_array[ii][iii] = *chk;  
   }
    char_array[ii][iii] = '\0';
    upp_low(char_array[ii]);

  def_matrix_dimension[ii] = iii;
  }






 /* 
     The matrix is "just" a pointer to the first element.  Here we
     create a matrix of integers, but you can of course create a
     matrix of any type, such as structs.
  */
  struct mode **matrix;
  
  int nrows, ncols;
  int ir, ic;
  
  /*
    Initialize and allocate a M*N matrix.
  */
  nrows = def_matrix_dimension[1];
  ncols = def_matrix_dimension[2];
  matrix = malloc (nrows * sizeof(struct mode*));
  if (NULL == matrix) {
    err (EXIT_FAILURE, "malloc");
  }
  
 for (ir = 0; ir < nrows; ++ir) {
    matrix[ir] = malloc (ncols * sizeof(struct mode));
    if (NULL == matrix[ir]) {
      err (EXIT_FAILURE, "malloc row %d", ir);
    }
  }



// fill the first row and first column with  -5 penalty
matrix[0][0].arry = 0;
 for (ir = 1; ir < nrows ; ++ir){
  matrix[ir][0].arry = matrix [ir - 1][0].arry - 5;matrix[ir][0].def[0] = ' ';matrix[ir][0].def[1] = ' '; matrix[ir][0].def[2] = 'd';

 }
 for (ic = 1; ic <ncols ; ic++){
 matrix[0][ic].arry = matrix[0][ic - 1].arry - 5;matrix[0][ic].def[0] = ' ';matrix[0][ic].def[1] = 'i';matrix[0][ic].def[2] = ' ';


  }

//===========================================================================




  /*
    Fill the matrix with some example values. Notice that the array
    index is one-dimensional: in order to access the element at
    (ir,ic) you have to translate it into the index ic+ir*ncols.
  */
int score = -1000,score_left= -1000,score_up= -1000;  

for (ir = 1; ir < nrows; ++ir) {
    for (ic = 1; ic < ncols; ++ic) {

      if (char_array[1][ir] == char_array[2][ic])
        {
	score = matrix[ir-1][ic-1].arry + 10;
        }
        else
         { 
         if (isvowel(char_array[1][ir]) && isvowel(char_array[2][ic])) {
           score = matrix[ir-1][ic-1].arry - 2;
          }
         if (! (isvowel(char_array[1][ir]) && isvowel(char_array[2][ic]))){
               score = matrix[ir-1][ic-1].arry - 4;
              }
         if (((!isvowel(char_array[1][ir])) && isvowel(char_array[2][ic])) || (isvowel(char_array[1][ir]) && (!isvowel(char_array[2][ic]))))
            {
               score = matrix[ir-1][ic-1].arry - 10;
             }   
          }
                        
        score_left=matrix[ir][ic-1].arry - 5;
        score_up=matrix[ir-1][ic].arry - 5;
     
       
          matrix[ir][ic]=find_max(score,score_left,score_up);
          score = -1000;score_left = -1000;score_up = -1000; 
     }
}
/* print the matrix as a nice table with a header row that lists the
    column indices, a header column which shows the row indices, and
    the values right-aligned into a field of width 3.
  */
 printf ("  |");
  for (ic = 0; ic < ncols; ++ic) {
    printf ("   %3c", char_array[2][ic]);
  }
  printf ("\n--+");
  for (ic = 0; ic < ncols; ++ic) {
    printf ("------");
  }
 printf("\n");
  for (ir = 0; ir < nrows; ++ir) {
    printf ("%c |", char_array[1][ir]);
    for (ic = 0; ic < ncols; ++ic) {
      printf ("   %3d", matrix[ir][ic].arry);
    }
    printf("\n   ");
    for (ic = 0; ic < ncols; ++ic)
    printf("   %3s",matrix[ir][ic].def);
    printf ("\n\n");
  }
  
  /*
    Free the memory used by the matrix. Notice that now we first have
    to free each individual row, then the array of row pointers.
  */
  
//========================================================================================
//              trace back and print the output
//=========================================================

//===========================================
ii=traceback(nrows,ncols,matrix,char_array);
//===========================================
int j;
char_array[1][0]=' ';
char_array[2][0]=' ';
printf ("\n\n input source      = %s \n" 
        " input destination = %s \n\n",char_array[1],char_array[2]);
printf("output source      =  ");
for (j=ii;j>0; j--){

printf (" %c",char_array[4][j]);

}

printf ("\n");
printf("output destination =  ");
for (j=ii ;j>0; j--){

printf (" %c",char_array[5][j]);
}
printf ("\n");

for (ir = 0; ir < nrows; ++ir)
    free (matrix[ir]);
  free (matrix);
  
  return 0;
}

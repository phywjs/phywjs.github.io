/*---------------------------------------------------------------------------- 
               E I G E N   V A L U E S   A N D   V E C T O R S
  ----------------------------------------------------------------------------

   This module handle diagonalization of a Hamiltonian, calling
   Lapack(3.0) routines (from C).  Unfortunately, calling Fortran from C
   is machine-dependent.  The trick does not work with (DEC) C++ compiler.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

typedef double real;
#define SIZE_OF_REAL 8

#undef OSF                               /* Digital/Compaq alpha Unix or OSF */
#define LINUX                                               /* red-hat linux */
                                            /* external lapack routine names */
#if (defined(LINUX) || defined(OSF))

#if (SIZE_OF_REAL == 4)         /* choose single or double precision version */
#define lapackeigen  ssyevr_
#elif (SIZE_OF_REAL == 8)
#define lapackeigen  dsyevr_
#else
#define lapackeigen
#endif

#endif

void lapackeigen(char *JOBZ, char *RANGE, char *UPLO,
                  int *N, real *A, int *LDA, 
                  real *VL, real *VU, int *IL, int *IU,
                  real *ABSTOL, int *M, real *W, real *Z, int *LDZ, int *ISUPPZ,
                  real *WORK, int *LWORK, int *IWORK, int *LIWORK, 
                 int *INFO);


/* Solving standard eigenvalue problem
   h psi = epsilon psi
   h is dim x dim matrix, 
   epsilon is vector of dimension dim,
   and psi is eigenvector of dim by nov.  Each column of psi is an
   eigenvector.  Only the first nov smallest eigenvalues/vectors are
   computed.  The mapping of 1D array to 2D matrix must follow
   Fortran convention of column major (i.e. row index runs the fastest).
*/
void eigensystem(const real *h, real *epsilon, real *psi, int dim, int nov)
{
   char JOBZ, RANGE, UPLO;
   real VL, VU, ABSTOL;
   int INFO, LDA, IL, IU, LWORK, N, M, LIWORK, LDZ, i;
   real *A, *WORK;
   int *ISUPPZ, *IWORK;

                                                   /* prepare to call Lapack */
   JOBZ = 'V';                   /* 'N' eigenvalue only, 'V' value & vectors */
   if(nov < dim) {
      RANGE = 'I';             /* 'A' all values, 'I' range given by [IL,IU] */
   } else {
      RANGE = 'A';                        /* 'A' for all eigenvalues/vectors */
      assert(nov == dim);
   }
   UPLO = 'U';                        /* Upper or Lower triangles are stored */
   N = dim;                                              /* actual dimension */
   LDA = dim;                                           /* leading dimension */
   VL = 0;                                   /* lower limit of eps, not used */
   VU = 1e20;                                /* upper limit of eps, not used */
   IL = 1;                                  /* lower limit of ith eigenvalue */
   IU = nov;                                /* upper limit, must >0 && < dim */
   ABSTOL = 1.0e-12;                                    /* absolute accuracy */
   LDZ = dim;                               /* eigenvector leading dimension */
   LWORK = 40*dim;                                 /* what is optimal LWORK? */
   LIWORK = 20*dim;                                            /* IWORK size */

   WORK = (real *) calloc( LWORK, sizeof(real) );        /* initialized to 0 */
   IWORK = (int *) calloc( LIWORK, sizeof(int) );    
   A = (real *) malloc( dim*dim*sizeof(real) );
   ISUPPZ = (int *) malloc( 2*dim*sizeof(int) );             /* support of Z */
   assert(WORK != NULL);
   assert(IWORK != NULL);
   assert(A != NULL);
   assert(ISUPPZ != NULL);
   
   for(i = 0; i < dim*dim; ++i) {
      A[i] = h[i];
   }

   lapackeigen(&JOBZ, &RANGE, &UPLO,
               &N, A, &LDA, 
               &VL, &VU, &IL, &IU,
               &ABSTOL, &M, epsilon, psi, &LDZ, ISUPPZ,
               WORK, &LWORK, IWORK, &LIWORK, 
               &INFO);

   assert(M == nov);                         /* M is total eigenvalues found */
   if(INFO != 0) { 
      fprintf(stderr, "INFO = %d\n", INFO);
      fprintf(stderr, "Lapack routine returns with error!");
   }

   free(ISUPPZ);
   free(A);
   free(IWORK);
   free(WORK);
}

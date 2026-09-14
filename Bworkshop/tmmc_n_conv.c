/* Fairly general program to study TMMC dynamics

   Flat histogram/equal hits, with or without N-fold way, based on 
   <N(E,DE)>, for general Q-state model in D dimensions with uniform
   or random interactions.  Default model is ferromagnetic Q-state Potts, 
   define SPIN_GLASS for spin glass, and ANTI_FERRO for antiferromagnets. 
   Define N_FOLD for N-fold dynamics, otherwise single-spin flip with
   rejection.  Fourteen difference algorithms, algo = -1, 0, 1, ...,12, 
   see code for detail.   

   This code is optimized to compute the error for a given Monte Carlo
   step -- testing rate of convergence.

   22 Oct 1999, Wang Jian-Sheng, at wangjs@cz3.nus.edu.sg.
   27 Feb 2000, revised to compute errors in n(E).

*/

#define  NDEBUG                       /* #define will turn off assertion code */
#define  N_FOLD                      /* undef give original single-spin flip */
#undef  CANONICAL                    /* define give canonical emsemble at Tr */
#undef  SPIN_GLASS
#undef  ANTI_FERRO

#define UNIX                                        /* define use 64-bit lcg */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
                                                        /* macro definitions */
#define  Q  2                                      /* number of Potts states */
#define  D  2                           /* dimension of lattice, D = 1,2,3,4 */
#define  L  50                                        /* lattice linear size */
#define  MCTOT   100000000                    /* total Monte Carlo steps */
#define  MCDIS   0                       /* steps discarded in the beginning */
#define  NLOOP   9                                   /* number of samples */
#define  TMAX  2                        /* correlation time from 0 to TMAX-1 */
#define  Tr     2.0                  /* temperature, for Metroplis flip only */
#define  SEED    11                   /* seed for spin-glass coupling config */

#if D==1
#define  N  (L)
#elif D==2
#define  N  (L*L)                                   /* total number of spins */
#elif D==3
#define  N  (L*L*L)
#else
#define  N  (L*L*L*L)
#endif

#define  Z  (2*D)                                     /* coordination number */

#if (Q==2)                                                     /* Ising type */ 
#define CLASS (Z+1)                              /* CLASS=(Z+1) only for Q=2 */
#define ETOT ((D*N)/2+2)  
#else
#define CLASS ((2*Z)+1)                                   /* for Potts Q > 2 */
#define ETOT ((D*N)+2)                      /* total number of energy levels */
#endif

#define MOVETOT  (N*(Q-1))      /* max number of possible move in each class */

typedef  signed char  Spin;                   /* data type for spin variable */
typedef  struct { int state; int site;} Move;         /* move state q site i */
typedef  struct { int clss; int addr;} Table;         /* pointer to the list */ 

                                                                  /* globals */
Spin s[N];                                           /* Potts spin, 0 to Q-1 */
Spin J[Z][N];                                                      /* J_{ij} */
Table tolist[Q][N];                   /* map a site i to the address on list */
Move list[CLASS][MOVETOT];              /* lists of moves each delta E class */
int top[CLASS];                      /* top of first available entry in list */
double  mat[ETOT][CLASS+3];           /*  statistics, mat[i][j]=T[i+(j-D),i] */
double  mat_old[ETOT][CLASS+3];                 /* to remember at half MCDIS */
double  lnW[ETOT];                                  /* log density of states */
double  lnW_ex[ETOT];                  /* log density of states, exact value */
double  lnn[ETOT];                     /* log density of states, exact value */
int  buffer[2][TMAX];                     /* circular buffer for correl time */
double  f[2][TMAX];                              /* accumulated <e(t)e(t+T)> */
int  tpt=0;                    /* pointer to the most recent entry to buffer */
int E_cur;                                           /* current total energy */
int E_min, E_max;                           /* current min and max of energy */
double t_min, t_max;                        /* clk when E_min/max is reached */
static double clk = 0.0;                           /* "clock tick", MC moves */
double T_tunnel = 0.0;                            /* duration between tunnel */
double last_tunnel = 0.0;                              /* clk of last tunnel */
double n_tunnel = 0.0;                            /* number of tunnel events */
int look_for_max = 1;                       /* are we looking for the E_max? */
int Shift_up;                                    /* energy scale shifted up? */
int algo;                                       /* which algorithms -1 to 12 */
double ave_t[ETOT];                               /* return time to energy E */
double last_t[ETOT];
double num_t[ETOT];

                                                      /* function prototypes */
void init(void);
void monte_carlo(int step);
int e_change(int i, int q);
void metrop(int step);
int energy(void);
int magnetization(void);
void correlation(int e, int mag);
void neighbor(int i, int nn[ ]);
void transM(int etot);
double drand64(void); 
void srand64(long seed);
void check(void);
void pofe_OPT_isn(int argc, char **argv, double lnP[]);

int main(int argc, char **argv)
{
   int  i, j, mc, ie, e, mag;
   int  Di, Qi, Li, mcin;
   double  mi, wi;
   double return_time; 
   double tn, tn_n, tn_av, tn_av2, e_min_av, e_min_av2, e_max_av, e_max_av2;
   double tg, tg_min_av, tg_min_av2, tg_max_av, tg_max_av2;
   double err, err1, err2, err_av, err_av2;
   int E_min_old, E_max_old, loop;
   char char_s[122];
   char *argvp[3];
   double din, mcf;
   FILE  *fp;

#ifdef SPIN_GLASS
   char model[] = "Potts-Glass";
#elif defined(ANTI_FERRO)
   char model[] = "AntiFerro-Potts";
#else
   char model[] = "Potts";
#endif
 
   tn_n = 0.0;
   tn_av = 0.0;
   tn_av2 = 0.0;
   e_min_av = 0.0;
   e_min_av2 = 0.0;
   e_max_av = 0.0;
   e_max_av2 = 0.0;
   tg_min_av = 0.0;
   tg_min_av2 = 0.0;
   tg_max_av = 0.0;
   tg_max_av2 = 0.0;
   err_av = 0.0;
   err_av2 = 0.0;

   if (argc < 3 || argc > 4) {
      fprintf(stderr, "Use: %s lnWfile Tfile > ffile\n", argv[0]);
      fprintf(stderr, "or: %s lnWfile Tfile algo > ffile\n", argv[0]);
      exit(1);
   }
   if(argc == 4) { 
      algo = atoi(argv[3]);
   } else {
      assert(argc == 3);
      algo = 0;
   }

   fp = fopen(argv[1], "r");                 /* read file contain exact ln n */
   assert(fp != NULL);
   printf("file %s opened\n", argv[1]);
   fgets(char_s, 120, fp);
   puts(char_s); 
   fscanf(fp, "%d%d%d", &Di, &Qi, &Li);
   assert(D == Di);
   assert(Q == Qi);
   assert(L == Li);
   fscanf(fp, " {");
   for(i=0; i< ETOT-2; ++i) {
      fscanf(fp, "%lf, ", &lnW_ex[i]);
      printf("%g\n", lnW_ex[i]);
   } 
   fscanf(fp, "%lf", &lnW_ex[ETOT-2]);
      printf("%g\n", lnW_ex[ETOT-2]);
   fclose(fp);

   if(algo == -1) {
      for(i = 0; i < ETOT; ++i) { 
         lnW[i] = lnW_ex[i];
      }
   }


   printf("%s, D= %d Q= %d L= %d MCSTEP= %d MCDIS= %d TMAX= %d algo= %d",
           model, D, Q, L, (MCTOT-MCDIS), MCDIS, TMAX, algo);
#ifdef CANONICAL
   printf(", canonical\n");
#else
   printf("\n");
#endif
#ifndef N_FOLD 
   printf("Single-Spin-Flip, NO N-FOLD\n");
#endif

   loop = 0;
Loop:

   init();

   for(mc = 0; mc < MCTOT; ++mc) {

#ifdef CANONICAL
      metrop(N);
#else
      monte_carlo(N);
#endif
      if(mc == MCDIS/2) {                 /* at half MCDIS, make a data dump */
         for(i = 0; i < ETOT; ++i) {
            for(j = 0; j <= CLASS+2; ++j) {
               mat_old[i][j] = mat[i][j];
            }
         }
      } else if (mc == MCDIS) {
         for(i = 0; i < ETOT; ++i) {
            for(j = 0; j <= CLASS+2; ++j) {
               mat[i][j] -= mat_old[i][j];
            }
         }
         T_tunnel = 0.0;
         n_tunnel = 0.0;
         E_min_old = E_min;
         E_max_old = E_max;
      }
      if(mc >= MCDIS) {
         ie = energy();
#if (Q==2)
         ie -= Shift_up;
         e = ie/4;
         assert((ie%4) == 0);
#else
         e = ie/2;
         assert((ie%2) == 0);
         assert(Shift_up == 0);
#endif
         assert(e >=0 && e < ETOT);
         assert(e == E_cur);
         mag = abs(magnetization());
         correlation(e, mag);
#ifdef CANONICAL 
         transM(e);
#endif
      }
   }

   fp = fopen(argv[2], "w");
   fprintf(fp, "%s, D= %d Q= %d L= %d MCSTEP= %d MCDIS= %d TMAX= %d aglo=%d",
           model, D, Q, L, (MCTOT-MCDIS), MCDIS, TMAX, algo);
#ifdef CANONICAL
   fprintf(fp, ", canonical\n");
#else
   fprintf(fp, "\n");
#endif

   fprintf(fp, "%d %d %d %d\n", D, Q, L, (MCTOT-MCDIS));
   for(i = 0; i < ETOT; ++i) {
      if( mat[i][CLASS+1] != 0.0) {
         fprintf(fp, "%g ", (double) i + 0.25 * Shift_up);
         for(j = 0; j <= CLASS+2; ++j) {
            fprintf(fp, "  %.16g", mat[i][j]);
         }
         if (num_t[i] > 0) { 
            return_time = ave_t[i]/num_t[i];
         } else {
            return_time = 0;
         }
         fprintf(fp, "  %.16g\n", return_time);
      }
   }
   fclose(fp);

   if (1) {                           /* turn on/off tunnel time calculation */

   if (loop % ( NLOOP/10 + 1 ) == 0) {
     printf("Tunneled %g times, ", n_tunnel);
   }
     if(n_tunnel > 0.0) tn = T_tunnel/n_tunnel/N;

     tn_n += 1.0;
     tn_av += tn;
     tn_av2 += (tn*tn);


   if (loop % ( NLOOP/10 + 1 ) == 0) {
     printf("Ave tunneltime = %g ", tn_av/tn_n);
     if (tn_n >= 2.0) {
        printf("+/- %g ", 
            sqrt( (tn_av2/tn_n - (tn_av/tn_n)*(tn_av/tn_n) )/(tn_n-1)));
        printf(" Over samples = %g\n", tn_n);
     } else {
        printf("\n");
     }
   }

     e_min_av += (E_min + 0.25*Shift_up);
     e_min_av2 += (E_min + 0.25*Shift_up)*(E_min + 0.25*Shift_up);
     e_max_av += (E_max + 0.25*Shift_up);
     e_max_av2 += (E_max + 0.25*Shift_up) * (E_max + 0.25*Shift_up);

   if (loop % ( NLOOP/10 + 1 ) == 0) {
     printf("E_min = %g ", e_min_av/tn_n);
     if (tn_n >= 2.0) {
        printf("+/- %g ", 
           sqrt( (e_min_av2/tn_n-(e_min_av/tn_n)*(e_min_av/tn_n) )/(tn_n-1) ));
     }
     printf("E_max = %g ", e_max_av/tn_n);
     if (tn_n >= 2.0) {
        printf("+/- %g\n", 
           sqrt( (e_max_av2/tn_n-(e_max_av/tn_n)*(e_max_av/tn_n) )/(tn_n-1) ));
     } else {
        printf("\n");
     }
   }

     tg = t_min/N;                                     /* first passage time */
     tg_min_av += tg;
     tg_min_av2 += tg*tg; 
     tg = t_max/N;
     tg_max_av += tg;
     tg_max_av2 += tg*tg; 
  
   if (loop % ( NLOOP/10 + 1 ) == 0) {
     printf("t_min/max = %g/%g, ", t_min/N, tg);
     printf("tg_min = %g", tg_min_av/tn_n);
     if (tn_n >= 2.0) {
        printf(" +/- %g ", 
        sqrt( (tg_min_av2/tn_n-(tg_min_av/tn_n)*(tg_min_av/tn_n) )/(tn_n-1) ));
     }
     printf("tg_max = %g", tg_max_av/tn_n);
     if (tn_n >= 2.0) {
        printf(" +/- %g\n", 
        sqrt( (tg_max_av2/tn_n-(tg_max_av/tn_n)*(tg_max_av/tn_n) )/(tn_n-1) ));
     } else { 
        printf("\n");
     }
   }
   }


   argvp[0] = "a.out";
   argvp[1] = "-n";
   argvp[2] = argv[2];
   pofe_OPT_isn(3, argvp, lnn);
                                                   /* compute errors in n(E) */
   err = 0.0;
   for(i = 0; i < ETOT - 1; ++i) {
      if (i == 1 || i == ETOT - 3) continue;
      err += fabs( exp(lnn[i]-lnW_ex[i]) - 1.0 );
   }

   err = err/(ETOT-3);                               /* normalize, per n(E). */
   err_av += err;
   err_av2 += err*err;

   ++loop;

   err1 = err_av/loop;
   if (loop > 1) {
      err2 = sqrt( (err_av2/loop - err1*err1)/(loop-1) );
   }

   if (loop % ( NLOOP/10 + 1 ) == 0) {
      mcf = mc;
      printf("loop= %d, mc= %g, err= %g, err_av= %g", loop, mcf, err, err1);
      if(loop > 1) {
         printf(" +/- %g\n", err2);
      } else { 
         printf("\n");
      }
   }
   fflush(stdout);
   if (loop < NLOOP) goto Loop; 

   return 0;
}



/* This function monte_carlo() performs flat histogram Monte Carlo moves 
   with N-fold way of Boltz-Kalos-Lebowitz.  It picks a class according
   to total probability going to that class, pick a move in that class
   and perform the move without rejection.  The flip is 
   performed with probability min(1, N(E',-DE)/N(E,DE)), where
   E is current energy, E' is the new energy, DE = E'-E.  N is the total 
   number of spin, a macro definition, s[], Z is passed globally. 

   If N_FOLD is undefined, single-spin-flip is used.

   Data structure: 
   list[CLASS][MOVETOT] holds the moves for each class of given delta E,
   each list of class cl has top[cl] numbers.  Each move is characterized
   by two numbers {new state q, site i}, as a structure.  Given a value
   in list[] we can go back to the lattice of spins for updating.  
   tolist[Q][N] is a pointer to the member of valid move, i.e., move to
   new state q, at site i is on list[] at location tolist[q][i], which
   is also two numbers {class, addr}.  However for q == s[i], it is not
   a valid move and this pointer is set to NULL = {-1,-1}. 
*/


#define EPSILON 1.0e-13 

void monte_carlo(int step)
{
   int i, j, k, q, q0, cl, mc; 
   int top_tot, ads, Ep, kl, ql, set;
   int e, cl_new, sj;
   int nn[Z+1];
   Move tb;
   double Aa, Ab, a, b, r, xi, accept;
   double R[CLASS], P[CLASS], psum;

   for(mc = 0; mc < step; ++mc) {
    
      clk += 1.0;

      set = 0;
      if (fabs(mat[E_cur][CLASS+1]) < EPSILON) {         /* first time visit */
         for(cl = 0; cl < CLASS; ++cl) {
            mat[E_cur][cl] = top[cl];
         }
         mat[E_cur][CLASS] = 1.0;
         mat[E_cur][CLASS+1] = 1.0;
         set = 1;
      }

      accept = 0.0;
      top_tot = 0;
      for(cl = 0; cl < CLASS; ++cl) {   /* column CLASS is for normalization */
         Ep = E_cur + cl - CLASS/2;                        /* CLASS must odd */
         if( Ep >= 0 && Ep < ETOT && 
             fabs(mat[Ep][CLASS+1]) > EPSILON ) { /* int (CLASS/2) is center */
            a = mat[Ep][CLASS-cl-1]/mat[Ep][CLASS];     
            a = a/MOVETOT;                                   /* a = T(E'->E) */
            Aa = mat[Ep][CLASS]/mat[Ep][CLASS+1];                /* <1/A>_E' */
         } else {
            a = 0.0;
            Aa = 0.0;
         }
         if( fabs(mat[E_cur][CLASS+1]) > EPSILON ) {
            b = mat[E_cur][cl]/mat[E_cur][CLASS];            /* b = T(E->E') */
            b = b/MOVETOT;
            Ab = mat[E_cur][CLASS]/mat[E_cur][CLASS+1];           /* <1/A>_E */
         } else {
            b = 0.0;
            Ab = 0.0;
         }

         assert(a >= 0 && a <= 1.000001);
         assert(b >= 0 && b <= 1.000001);
         assert(Ab >= 0 && Ab >= 0);

         switch (algo) {

            case -1:

               if (Ep < 0 || Ep >= ETOT) {
                  r = 0.0;
               } else { 
                  a = lnW[E_cur] - lnW[Ep];
                  if (a > 0.0) {
                     r = 1.0;
                  } else {
                     r = exp(a);
                  } 
               }
               break;

            case 1:
              /* The ORIGINAL equal hit algorithm 
                 a(E->E') = min(1, <1/A>_E' T(E'->E)/ [ <1/A>_E T(E->E') ] ) 
               */
               a = a * Aa;
               b = b * Ab;
               if( fabs(a) < EPSILON || fabs(b) < EPSILON || a > b ) {
                  r = 1.0;
               } else { 
                  r = a/b;
               }
               break;

            case 2:
               /* equal hit, using `from' information only 
                  a(E->E') = <1/A>_E' T(E'->E)   
                */   
               a = a * Aa;
               if (fabs(a) < EPSILON ) {
                  r = 1.0;
               } else {
                  r = a;
               }
               break;

            case 3:
               /* equal hit, using `to' information only
                  a(E->E') = 1/( <1/A>_E T(E->E') )      
               */
               b = b * Ab;
               if (fabs(b) < EPSILON) { 
                  r = 1.0; 
               } else {
                  r = 1.0/b;
               }
               break;

            case 4:
               /* equl hit, but with
                  a(E->E') = 1/<1/A>_E, if <1/A>_E' T(E'->E) >= <1/A>_E T(E->E')
                           = T(E'->E)/( <1/A>_E T(E->E') ), else
               */    
               if ( fabs(Ab) < EPSILON || fabs(b) < EPSILON 
                                       || fabs(a) < EPSILON) {
                  r = 1.0; 
               } else { 
                 if ( a * Aa < b * Ab) { 
                    r = 1/Ab;
                 } else { 
                    r = a/(Ab * b);
                 }
               }
               break;

            case 5:
               /* multicanonical, very simple one,
                  a(E->E') = T(E'->E)
               */
               if (fabs(a) < EPSILON) {
                  r = 1.0;
               } else { 
                  r = a;
               }
               break;

            case 6:
               /* multicanonical, equal hits? 
                  a(E->E') = 1/T(E->E')
               */
               if(fabs(b) < EPSILON) { 
                  r = 1.0;
               } else {
                  r = 1/b;
               }
               break;

            case 7:
               /* multicanonical, modifed case 5
                  a(E->E') = T(E'->E)/(T(E->E') + T(E'->E) )
               */
               if (fabs(a) < EPSILON || fabs(a+b) < EPSILON) {
                  r = 1.0;
               } else { 
                  r = a/(a+b);
               }
               
               break;

            case 8:
               /* multicanonical, modifed case 6
                  a(E->E') = (T(E->E') + T(E'->E) )/ T(E->E')
               */
               if(fabs(b) < EPSILON || fabs(a+b) < EPSILON) { 
                  r = 1.0;
               } else {
                  r = (a+b)/b;
               }
               break;

             case 9:
               /* equal class,
                  a(E->E') = 1/t(E->E')
               */
                if(top[cl] == 0) { 
                   r = 1.0;
                } else {
                   r = MOVETOT/ (double) top[cl];  
                }
                break;  

            case 10:
               /* equal hit, using `to' information only
                  a(E->E') = <1/A>_E' / T(E->E') )      
               */
               if (fabs(b) < EPSILON || fabs(Aa) < EPSILON) { 
                  r = 1.0; 
               } else {
                  r = Aa/b;
               }
               break;


            case 11:
               /* equal hit, using `from' information only and   <1/A>_E
                  a(E->E') = (1/<1/A>_E) T(E'->E)   
                */   
               if (fabs(a) < EPSILON || fabs(Ab) < EPSILON ) {
                  r = 1.0;
               } else {
                  r = a/Ab;
               }
               break;

            case 12:
               /* equl hit, but with
                  a(E->E') = <1/A>_E', if <1/A>_E' T(E'->E) >= <1/A>_E T(E->E')
                           = <1/A>_E' T(E'->E)/( T(E->E') ), else
               */    
               if ( fabs(Ab) < EPSILON || fabs(b) < EPSILON 
                                       || fabs(a) < EPSILON) {
                  r = 1.0; 
               } else { 
                 if ( a * Aa < b * Ab) { 
                    r = Aa;
                 } else { 
                    r = Aa * a/b;
                 }
               }
               break;

            default:

              /* case 0: The ORIGINAL multicanonical algorithm 
                 a(E->E') = min(1, T(E'->E)/T(E->E') )   */

               if( fabs(a) < EPSILON || fabs(b) < EPSILON || a > b ) {
                  r = 1.0;
               } else { 
                  r = a/b;
               }
         } 
         r += 1.0e-13;

         R[cl] = r;
         P[cl] = (double) top[cl] * r / MOVETOT;
         accept += P[cl];                                  /* accept = 1 - R */
         top_tot += top[cl];
      }                                               /* R is rejection rate */

#ifndef N_FOLD
      accept = 1.0;
#endif

      if(!set) {
         for(cl = 0; cl < CLASS; ++cl) {
            mat[E_cur][cl] += top[cl]/accept;
         }
         mat[E_cur][CLASS] += 1.0/accept;           /* CLASS column is <1/A> */
         mat[E_cur][CLASS+1] += 1.0;                /* CLASS+1 column is <1> */
      }


#ifdef N_FOLD                        /* N-fold way of Bortz, Kalos, Lebowitz */

      for(cl = 0; cl < CLASS; ++cl) {
         P[cl] /= accept;
      }                              /* normalized P[] such that sum P[] = 1 */
      xi = drand64();
      psum = P[0];
      cl = 0;
      while( xi > psum ) {                     /* pick a cl according to P[] */
         psum += P[++cl];
      }
      k = drand64() * (double) top[cl];                       /* pick a move */
      i = list[cl][k].site;
      q0 = s[i];
      s[i] = list[cl][k].state;                             /* state updated */
      assert(q0 != s[i]);
      E_cur += cl - CLASS/2;                                /* update energy */
      mat[E_cur][CLASS+2] += accept;                /* CLASS+2 column is <A> */
      assert(cl < (CLASS-1) || fabs(psum-1.0) < 1.0e-10);
      assert(k < top[cl]);
      assert(top[cl] <= MOVETOT);

#else                                             /* normal single-spin flip */

      i = N * drand64();
      q0 = s[i];
      q = (Q-1) * drand64();
      if (q >= s[i]) ++q;
      cl = e_change(i, q);
      if ( drand64() < R[cl]) {
         s[i] = q;                                            /* update spin */
         mat[E_cur][CLASS+2] += 1;                  /* CLASS+2 column is <A> */
         E_cur += cl - CLASS/2;                             /* update energy */
      }

#endif

      assert(accept > 0.0);
      assert(top_tot == N*(Q-1));
      assert(cl >= 0 && cl < CLASS);
      assert(i >= 0 && i < N);
      assert(s[i] >= 0 && s[i] < Q);
      assert(E_cur >= 0 && E_cur < ETOT);

      if ( fabs(last_t[E_cur]) < EPSILON) {
         last_t[E_cur] = clk;
      } else {
         ave_t[E_cur] += (clk - last_t[E_cur]);
         num_t[E_cur] += 1;
         last_t[E_cur] = clk;
      }
      if(E_cur < E_min) { 
         E_min = E_cur;
         t_min = clk;
      }
      if(E_cur > E_max) { 
         E_max = E_cur;
         t_max = clk;
      }

      if(look_for_max) {
         if(E_cur == E_max) { 
             T_tunnel += clk - last_tunnel;
             last_tunnel = clk;
             n_tunnel += 1.0;
             look_for_max = 0;
          } 
       } else {
         if(E_cur == E_min) { 
             T_tunnel += clk - last_tunnel;
             last_tunnel = clk;
             n_tunnel += 1.0;
             look_for_max = 1;
          } 
      }
      
      if (s[i] == q0) continue;     /* state did not change, not need update */

      neighbor(i, nn);                           /* find neighbors of site i */
      nn[Z] = i;

      for(j = 0; j <= Z; ++j) {                    /* add new entry to table */
         k = nn[j];                         
         for(q = 0; q < Q; ++q) {
            cl = tolist[q][k].clss;
            ads = tolist[q][k].addr;
            if (j == Z) {
               if (s[i] == q) {
                  cl_new = -2;
               } else {
                  cl_new = e_change(k, q);
               }
            } else {
               if(cl == -1) continue;
               sj = s[k];
               e = J[j][i] * ( (sj == s[i]) - (q == s[i]) 
                              - (sj == q0) + (q == q0) );
#if (Q==2)
               assert(e % 2 == 0);
               e = e/2;
#endif
               cl_new =  cl + e;
               assert(cl_new >= 0 && cl_new < CLASS);
            }
            if (cl_new != cl) {
               if (cl != -1) { 
                  list[cl][ads] = list[cl][--top[cl]];
                  kl = list[cl][ads].site;
                  ql = list[cl][ads].state;
                  tolist[ql][kl].clss = cl;
                  tolist[ql][kl].addr = ads;
                 assert(top[cl] >= 0);
               }
               if(s[k] == q) {
                  tolist[q][k].clss = -1;         /* this means NULL pointer */
                  tolist[q][k].addr = -1;
               } else {
                  tolist[q][k].clss = cl_new;
                  tolist[q][k].addr = top[cl_new];
                  tb.state = q;
                  tb.site = k;
                  list[cl_new][top[cl_new]++] = tb;
               }
            }
         }
      }
/**  Commented out, only for debug checking:
      check(); 
**/
   }
}

/* What is the class number cls (mapped by energy) if site
   i is changed to state q from s[i].
*/
int e_change(int i, int q)
{
   int nn[Z];
   int e, si, sj, j, cls;

   assert(s[i] != q);

   neighbor(i, nn);
   si = s[i];
   e = 0;
   for(j = 0; j < Z; ++j) {
      sj = s[nn[j]];
      e += J[j][i] * ( (si == sj) - (q == sj) );
   }

#if (Q==2)
   cls = (e+Z)/2;
#else
   cls = e + Z;
#endif
   assert(cls >= 0 && cls < CLASS);
   
   return cls;
}

/* do Metropolis canonical Monte Carlo, T, N, CLASS are macros. */
void metrop(int step)
{
   int mc, i, q, e, de, cl;

   for(mc = 0; mc < step; ++mc) {
      i = N * drand64();
      q = (Q-1) * drand64();
      if (q >= s[i]) ++q;
      cl = e_change(i, q);
      de = cl - (CLASS/2);
#if (Q==2)
      e = de * 4;
#else
      e = de * 2;
#endif
      if (e <= 0 || drand64() < exp(-e/Tr)) {
         s[i] = q;
         E_cur += de;
         mat[E_cur][CLASS+2] += 1.0;
      }
   }
}



/* Neighbor returns in the array nn[ ] the neighbor sites of i.  The sites
are labelled sequentially, starting from 0.  It works for any hypercubic
lattice.  Z (=2*D) is the coordination number, passed as a macro defintion.
L is linear size, also passed as a macro definition. */

void neighbor(int i, int nn[ ])
{
   int j, r, p, q;

   r = i;
   p = 1 - L;
   q = 1;

   for(j = 0; j < Z; j += 2) {
      nn[j] = (r + 1) % L == 0 ? i + p : i + q;
      nn[j+1]     = r % L == 0 ? i - p : i - q;
      r = r/L;
      p *= L;
      q *= L;
   }
}


/* This function computes the energy of the current configuraion s[].
   The energy is defined as E = - sum J_{ij}(2 delta (s[i], s[j])-1),
   However, the value is rescaled to take integers from 0 only.
*/

int energy(void)
{
   int i, j, ie, si; 
   int nn[Z];
   
   ie = 0;
   for(i = 0; i < N; ++i) {
      neighbor(i, nn);
      si = s[i];
      assert(si >= 0 && si < Q);
      for(j = 0; j < Z; j += 2) {        /* look at positive directions only */
         ie += J[j][i] * (1 - 2 * (si==s[nn[j]]));
         assert(J[j][i] == 1 || J[j][i] == -1);
      }
   }

   return (N*D+ie);
}

int magnetization(void)
{
   int i, mag;

   mag = 0;
   for(i = 0; i < N; ++i) {
      mag += (s[i]==0);
   }
   return mag;
 }


/* Compute mat[] when using canonical simulation */
void transM(int etot)
{
   int i, q, cl; 
   int ne[CLASS];

   for(cl = 0; cl < CLASS; ++cl) {
      ne[cl] = 0;
   }
   for(i = 0; i < N; ++i) {
      for(q = 0; q < Q; ++q) { 
         if(s[i] == q) continue;
         cl = e_change(i, q);
         assert(cl >= 0 && cl < CLASS);
         ++ne[cl];
      }
   }
   for(cl = 0; cl < CLASS; ++cl) {
      mat[etot][cl] += ne[cl];
   }
   mat[etot][CLASS+1] += 1.0;
   mat[etot][CLASS] += 1.0;
   assert(etot >=0 && etot < ETOT);
}

/* correlation function of energy and |mag|.  tpt, buffer[], f[] are 
   global */

void correlation(int e, int mag)
{
   int t;

   tpt = (tpt+1) % TMAX;
   buffer[0][tpt] = e;
   buffer[1][tpt] = mag;
   for(t = 0; t < TMAX; ++t) {
      f[0][t] += (double) e * buffer[0][(tpt-t+TMAX)%TMAX];
      f[1][t] += (double) mag * buffer[1][(tpt-t+TMAX)%TMAX];
   }
}


/* Do initialization of spins, coupling, mat[], etc */
void init(void)
{
   int i, j, k;
   int q, cl;
   int nn[Z];
   int etot, ie;
   Move tb;
   static int cnt  = 0;

   if(cnt == 0) {
      srand64(SEED);
   }

   for(i = 0; i < N; ++i) {
      if(cnt == 0) {
         s[i] = 0;                               /* s[i] = 0, 1, 2, ..., Q-1 */
      }
      for(k = 0; k < Z; k += 2) {            /* over each positive direction */
#ifdef SPIN_GLASS
         J[k][i] = (drand64() < 0.5) ? 1 : -1;            /* +/-J spin glass */
#elif defined(ANTI_FERRO)
         J[k][i] = -1;
#else 
         J[k][i] = 1;  
#endif
      }
   }

   for(i = 0; i < N; ++i) {
      neighbor(i, nn);
      for(k = 1; k < Z; k += 2) {                 /* each negative direction */ 
        J[k][i] = J[k-1][nn[k]];                  /* so that J_{ij} = J_{ji} */
      }
   }

   if(cnt == 0) {
      srand64(time(NULL));
   }

   ie = energy();
#if (Q==2)
                      /* for spin glass, delta E = 4, but E can shifted by 2 */
   Shift_up = ie % 4;
   ie -= Shift_up;
   etot = ie/4;
   assert((ie%4) == 0);
#else
   Shift_up = 0; 
   etot = ie/2;
   assert((ie%2) == 0);
#endif
   assert(etot >= 0 && etot < ETOT);
   E_cur = etot;
   E_min = E_max = E_cur;
   t_min = t_max = 0.0;
/*
   printf("E_min %d %d\n", 0, E_min);
   printf("E_max %d %d\n", 0, E_max);
*/
   clk = 0.0;
   T_tunnel = 0.0;
   last_tunnel = 0.0;
   n_tunnel = 0.0;

   for(j = 0; j < CLASS; ++j) {
      top[j] = 0;                                     /* empty for all lists */
   }

   for(i = 0; i < N; ++i) {
      for(q = 0; q < Q; ++q) {
         if(s[i] == q) {
            tolist[q][i].clss = -1;
            tolist[q][i].addr = -1;
            continue;                                 /* skip no change case */
         }
         cl = e_change(i, q);
         tolist[q][i].clss = cl;
         tolist[q][i].addr = top[cl];
         tb.state = q;
         tb.site = i;
         list[cl][top[cl]++] = tb;
      }
   }

   for(i = 0; i < ETOT; ++i) {
      for(j = 0; j <= CLASS+2; ++j) {
         mat[i][j] = 0.0;
      }
      ave_t[i] = 0.0;
      last_t[i] = 0.0;
      num_t[i] = 0.0;
   }

   for(i = 0; i < TMAX; ++i) {
      buffer[0][i] = 0;
      buffer[1][i] = 0;
      f[0][i] = 0.0;
      f[1][i] = 0.0;
   }

   cnt = 1;
}

/* Check table and tolist are correct */

void check(void) 
{
   int i, j, q, top_tot;
   int find, cl, cl0, j0;

   for(i = 0; i < N; ++i) {
      assert(s[i] >= 0 && s[i] < Q);
      for(j = 0; j < Z; ++j) {
         assert(J[j][i] == 1 || J[j][i] == -1);
      }
      for(q = 0; q < Q; ++q) {
         if (q == s[i]) {
            assert(tolist[q][i].clss == -1);
            assert(tolist[q][i].addr == -1);
            continue;
         }
         cl = e_change(i, q);
         find = 0;
         for(j = 0; j < top[cl]; ++j) {
            find = (list[cl][j].state == q && list[cl][j].site == i);
            if (find) break;
         } 
         assert(find);
         assert(tolist[q][i].clss == cl);
         assert(tolist[q][i].addr == j);
         cl0 = tolist[q][i].clss;
         j0 = tolist[q][i].addr;
         assert(list[cl0][j0].state == q);
         assert(list[cl0][j0].site == i);
      }
   }
   top_tot = 0;
   for(cl = 0; cl < CLASS; ++cl) {
      top_tot += top[cl];
      assert(top[cl] >= 0 && top[cl] <= MOVETOT);
   }
   assert(top_tot == MOVETOT);

}

#ifdef UNIX

/* 64-bit random number generator */

static unsigned long long x = 1;

double drand64(void) 
{
   x = 6364136223846793005ll * x + 1ll;
   return (double) x * 5.4210108624275218e-20; 
}

void srand64(long seed)
{
   assert(sizeof(long long int) == 8);
   x = seed;
   printf("initial seed x = %d\n", (int) x);
}

#else

/* This is a C translate from a Fortran program of Bob.

C THE FOLLOWING IS A NEW (11/92) MODIFICATION OF WELL-TESTED 
C RANDOM NUMBER GENERATOR.
C THE MODIFICATIONS CONSIST OF REPLACING
C                  532 BY 1279
C      AND          37 BY 1063
C AND CHANGING THE MAXIMUM STRING FROM 
C                    4096 TO ????

C NO CHANGES IMPLEMENTED YET (11/27/92)

c The following random number generator tested well on 8/10/88
c  Time per random number on long runs 
c	is about 85 micro-seconds without   -f68881
c	is about 42 micro-seconds with    -f68881
c It is on file in fortran/rg/ran0.f
c It is on file in usr/tmp/bob/ran0.f


ran2 for normal use, ran0 for init, ran1 internal.
*/

static int max,n,mult,n2;
static int mults[5+1];
static double rlist[532+1];
static int table[128+1];
static int np1,np2;
static double amaxin;

double drand64(void)
{
   double x;
   
   np1=np1-1;
   if(np1<=0) np1=532;
   np2=np2-1;
   if(np2<=0) np2=532;
   x=rlist[np1]-rlist[np2];
   if(x<0.0) x=x+1.0;
   rlist[np1]=x;
   return x;
}


static void ran1(double aran[], int nran)
{
   int k, l, m, l2;
   int kmult;
/* c      nran must not be greater than 4096 */

   for(k = 1; k <= nran; ++k) {
      kmult=k-(k/5)*5+1;
      mult=mults[kmult];
      n=n*mult;
      l=n/32768;
      n=n-l*32768;
      m=n/256 + 1;
      n2=table[m];
      aran[k]=n2*amaxin;
      n2=n2*mult;
      l2=n2/32768;
      n2=n2-l2*32768;
      table[m]=n2;
   }
}


void srand64(long int iseed)
{
   double aran[4096+1];
   int next, lext, np3;
   double x, fact, fact2;
   int j, k, l;

   mults[1]=189;
   mults[2]=187;
   mults[3]=195;
   mults[4]=181;
   mults[5]=197;

   n=483;

   next= (iseed >= 0) ? iseed : -iseed;
   lext=next/32766;
   next=next-lext*32766+1;
   n=(next/2)*2+1;
   max=32768;
   amaxin=1.0/max;
   mult=197;
   printf("random numbner parameters are %d, %d, %d\n", n, max, mult);

   for(j=1; j <= 128; ++j) {
      n=n*mult;
      l=n/32768;
      n=n-l*32768;
      table[j]=n;
   }

   np1=532;
   np2=37;
   fact=1.0/512.0;
   fact2=fact*fact;
   np3=3*532;
   ran1(aran,np3);
   for(k=1; k <= 532; ++k) {
      x=aran[k]-aran[k+532]*fact-aran[k+2*532]*fact2;
      if(x<=0.0) x=x+1.0;
      rlist[k]=x;
   }
   return;
}

#endif



/* Compute n(E) (actually ln n(E)), specialised for Ising model.
   30 July 99, revised 7 March 2000, Wang JS */

void pofe_OPT_isn(int argc, char **argv, double lnP[])
{
   int e, etot, i, k, Dv, Qv, Lv, mcs, class;
   int D_old, Q_old, L_old, first_i;
   int c, n, Shift_up, limit, iter, neqn;
   int file_no, file_begin; 
   int read_beta, read_no, read_lnW, read_n;
   int read_r, range;
   double mi;
   double sum, psum, avn, ave; 
   double r, r2, t, tp;
   double a, Nv, norm, max, beta;
   double F,  F_old, dF, del; 
   double Tmc[ETOT][CLASS+1];
   double T[ETOT][CLASS+1];
   double R[ETOT][CLASS+1];
   double Rav[ETOT][CLASS+1] ;
   double Rav2[ETOT][CLASS+1];
   double sig[ETOT][CLASS+1];
   double lnw[ETOT];
   double m[CLASS+1];
   double sT[CLASS+1];
   int exact[ETOT];

   char s[500];
   FILE *fp;

   if(argc == 1) { 
      printf("Use: %s [-b beta_value] [-i wfile] [-n] [-r #] file [files]\n", argv[0]);
      printf("-b betavalue, -i input lnW file, -n for normalization, -r range\n");
      exit(0);
   }
   file_begin = 1;
   read_beta = (argv[1][0] == '-') && (argv[1][1] == 'b');
   if (read_beta) {
      beta = atof(argv[2]);
      file_begin += 2;
   } else {
      beta = 0.0;
   }
   read_lnW = (argv[file_begin][0] == '-') && (argv[file_begin][1] == 'i');
   if (read_lnW) {
      file_begin += 2;
      read_lnW = file_begin - 1;
   } 
   read_n = (argv[file_begin][0] == '-') && (argv[file_begin][1] == 'n');
   if (read_n) {
      ++file_begin;
   }
   read_r = (argv[file_begin][0] == '-') && (argv[file_begin][1] == 'r');
   if (read_r) {
      range = atoi(argv[file_begin+1]);
      file_begin += 2;
   } else {
      range = 0;
   } 
/*
   printf("smooth range [%d,%d]\n", -range, range);
*/
   
   read_no = 0;
   for(file_no = file_begin; file_no < argc; ++file_no) {

   ++read_no;
/*
   printf("Reading file %s\n", argv[file_no]);
*/
   fp = fopen(argv[file_no], "r");
   assert(fp != NULL);

   fgets(s, 490, fp);
   fgets(s, 490, fp);

   sscanf(s, "%d%d%d%d", &Dv, &Qv, &Lv, &mcs);

   if (mcs == 0) mcs = 1;

   if(read_no == 1) {     /* read first file */
      etot = 1;
      i = 0;
      while (i < Dv) {
         etot = etot * Lv;
         ++i;
      }
      Nv = etot;
      etot = (etot*Dv)/2;
      if(Qv > 2) {
         etot = 2 * etot;
         ++etot;
         class  = 4*Dv + 1;
      } else if (Qv == 2) {
         ++etot;
         class  = 2*Dv + 1;
      } else {
        exit(0);
      }
      c = class/2;
      assert(etot < ETOT);

/*
      printf("D=%d, Q=%d, L=%d, etot=%d\n", Dv, Qv, Lv, etot);
*/
      D_old = Dv;
      Q_old = Qv; 
      L_old = Lv;

      for(i = 0; i < etot; ++i) {
         for(k = 0; k <= class; ++k) {
            T[i][k] = 0.0;
            Tmc[i][k] = 0.0;
            Rav[i][k] = 0.0;
            Rav2[i][k] = 0.0;
         }
      }

   } else {  /* end do if first time */
      assert(read_no > 1);
      assert(Dv == D_old);
      assert(Qv == Q_old);
      assert(Lv == L_old);
   }

   while( fgets(s, 490, fp) != NULL) { 
      if (class == 5) {
         sscanf(s, "%lf%lf%lf%lf%lf%lf%lf", 
                 &mi, m+0, m+1, m+2, m+3, m+4, m+5);
      } else if (class == 7) {
         sscanf(s, "%lf%lf%lf%lf%lf%lf%lf%lf%lf", 
                 &mi, m+0, m+1, m+2, m+3, m+4, m+5, m+6, m+7);
      } else if (class == 9) {
         sscanf(s, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf", 
                 &mi, m+0, m+1, m+2, m+3, m+4, m+5, m+6, m+7, m+8, m+9);
      } else {
        fprintf(stderr, "cannot handle class = %d\n", class);
      } 

      i = mi;
      if ((double) i != mi) { 
         Shift_up = 2.0*(mi-i);       /* half the value of Shit_up in main() */
      } else {
         Shift_up = 0;
      }
      assert(i >=0 && i < etot);
      for(k = 0; k <= class; ++k) {
         Tmc[i][k] = m[k];
         T[i][k] += Tmc[i][k];
      }
   }
   fclose(fp);

   for(e = 0; e < etot; ++e) {
      sum = 0.0;  
      for(k = 0; k < class; ++k) {
         sum += Tmc[e][k];
      }
      if (sum > 0) { 
         for(k = 0; k < class; ++k) {
            Tmc[e][k] = Tmc[e][k]/sum;
         }
      }
   }

   for(i = 0; i < etot; ++i) {
      for(k = 0; k < class; ++k) {
         if ( (i+k-c) >= 0 && (i+k-c) < etot) { 
            t = Tmc[i+k-c][class-1-k];
            tp = Tmc[i][k];
            if (tp > 0 && t > 0 ) { 
               r = log(t/tp);
            } else {
               r = 1.0e10;                    /* log 0, value does not exist */
            }
         } else {
            r = 1.0e10;
         }
         Rav[i][k] += r;
         Rav2[i][k] += r*r;
      }
      t = Tmc[i][class]/mcs;                         /* last comlumn is P(E) */
      if (t > 0 ) {
         r = log(t);
         Rav[i][class] += r;
         Rav2[i][class] += r*r;
      }
   }

   }    /* end read files */

   assert(read_no >= 1);
   if (read_no == 1) {
      for(i = 0; i < etot; ++i) {
         for(k = 0; k <= class; ++k) {
            sig[i][k] = 1.0;
         }
      }
      sig[2][0] = 0.0;
      sig[etot-3][4] = 0.0;
   } else if (read_no > 1) {
      for(i = 0; i < etot; ++i) {
         for(k = 0; k <= class; ++k) { 
            r = Rav[i][k]/read_no;
            r2 = Rav2[i][k]/read_no;
            del = r2 - r*r;
            if (del < 0) {
              if( fabs(del) > 1.0e-10 ) {
                 printf("WARNING: del = %g, Rav2 = %g, Rav = %g at (%d,%d)\n", 
                         del, Rav2[i][k], Rav[i][k], i, k);
              }
              del = 0.0;
            }
            sig[i][k] = del/(read_no-1);
         }
      }
   }


   if(Qv == 2) {
      for(e = 0; e < etot/2; ++e) {
         for(k = 0; k < class; ++k) {
            sT[k] = T[e][k] + T[etot-1-e-Shift_up][class-k-1];
         }
         for(k = 0; k < class; ++k) {
            T[e][k] = sT[k];
            T[etot-1-e-Shift_up][class-k-1] = sT[k]; 
         }
      }
/*
      printf("T symmetrized\n");
*/
   }

   for(e = 0; e < etot; ++e) {
      sum = 0.0;  
      for(k = 0; k < class; ++k) {
         sum += T[e][k];
      }
      if (sum > 0) { 
         for(k = 0; k < class; ++k) {
            T[e][k] = T[e][k]/sum;
         }
      }
   }

   if(Qv == 2) {
      for(e = 0; e < etot/2; ++e) {
         for(k = 0; k < class; ++k) {
            sT[k] = T[e][k] + T[etot-1-e-Shift_up][class-k-1];
         }
         for(k = 0; k < class; ++k) {
            T[e][k] = sT[k];
            T[etot-1-e-Shift_up][class-k-1] = sT[k]; 
         }
      }
   for(e = 0; e < etot; ++e) {
      sum = 0.0;  
      for(k = 0; k < class; ++k) {
         sum += T[e][k];
      }
      if (sum > 0) { 
         for(k = 0; k < class; ++k) {
            T[e][k] = T[e][k]/sum;
         }
      }
   }
/*
      printf("T symmetrized again\n");
*/
   }

/*
   printf("total average of data:\n");
   for(e = 0; e < etot; ++e) {
      printf("%d   ", e);
      for(k = 0; k <= class; ++k) {
         printf("%g ", T[e][k]);
      }
      printf("\n");
   }
*/

/* average sig to make it smooth */

   for(e = 0; e < etot; ++e) {
      for(k = 0; k <= class; ++k) {
         Tmc[e][k] = sig[e][k];
      }
   }

   for(e = 0; e < etot; ++e) {
      for(k = 0; k <= class; ++k) {
         avn = 0.0;
         ave = 0.0;
         for(i = -range; i <= range; ++i) {
            if (e+i >= 0 && e+i < etot) {
               avn += 1.0; 
               ave += Tmc[e+i][k];
            }
         }
         ave = ave/avn;
         if( sig[e][k] > 1.0e-14) {
            sig[e][k] = ave;
         }   
      }
   }

/*
   printf("Variance for R=T/T:\n");
   for(e = 0; e < etot; ++e) {
      printf("%d   ", e);
      for(k = 0; k <= class; ++k) {
         printf("%g ", sig[e][k]);
      }
      printf("\n");
   }
*/


   for(i = 0; i < etot; ++i) { 
      for(k = 0; k < class; ++k) {
         if ( (i+k-c) >= 0 && (i+k-c) < etot) { 
            t = T[i+k-c][class-1-k];
            tp = T[i][k];
            if (tp > 0 && t > 0 ) { 
               R[i][k] = log(t/tp);
            } else {
               R[i][k] = 1.0e30;              /* log 0, value does not exist */
            }
         } else {
            R[i][k] = 1.0e30;
         }
         if(T[i][class] > 0 ) {
            R[i][class] = log(T[i][class]);
         } else {
            R[i][class] = -1.0e30;
         }
      }
   }

   i = 0;
/*
   while( T[i][class] == 0.0 && i < etot) {
      ++i;
   }
*/
   first_i = i;
/*
   printf("first i =%d\n", first_i);
*/
   assert(i < etot);
   del = R[i][class];
   for(i = 0; i < etot; ++i) {
      lnP[i] = 0.0;
      R[i][class] -= del;          /* normalized ln P = 0, for ground states */
   }

/*
   printf("R= ln T/T and ln P(E):\n");
   for(e = 0; e < etot; ++e) {
      printf("%d   ", e);
      for(k = 0; k <= class; ++k) {
         printf("%g ", R[e][k]);
      }
      printf("\n");
   }
*/

   if(read_lnW) { 
     printf("Reading file %s\n", argv[read_lnW]);
     fp = fopen(argv[read_lnW], "r"); 
     fgets(s, 490, fp);
     sscanf(s, "%d%d%d", &Dv, &Qv, &Lv);
     assert(Dv == D_old);
     assert(Qv == Q_old);
     assert(Lv == L_old);
     for(i = 0; i < etot; ++i) {
        fscanf(fp, "%lf%lf", &mi, &lnw[i]);
        assert( i == ((int) mi) );
     }
     fclose(fp);

     del = lnw[first_i];
     for(i = 0; i < etot; ++i) {
        lnw[i] -= del;    
        sig[i][class] *= 1.0/(Lv*Lv);
     }

   }

   lnP[0] = lnP[etot-1] = log( (double) Qv);
   lnP[1] = lnP[etot-2] = -1e30;
   lnP[2] = lnP[etot-3] = log( (double) Nv*(Qv-1)*Qv );

   F = 1000.0;
   iter = 0;
Start:
   ++iter;
   if(iter == 1) {
      limit = c;
   } else {
      limit = class;
   } 
   for(i = 3; i < etot-3; ++i) { 
      n = 0;
      psum = 0.0;
      norm = 0.0;
      for(k = 0; k < limit; ++k) {
         if(k == c) continue;
          
         if ( R[i][k] < 1.0e25) {
            if ( fabs(sig[i][k]) < 1.0e-14 ) {        /* value known exactly */
               psum = lnP[i+k-c] + R[i][k];
               norm = 1.0;
               n = 1;
               exact[i] = 1;
               break;
            } else {                               /* use broad histogram eq */
               psum += (lnP[i+k-c] + R[i][k])/sig[i][k];
               norm += 1.0/sig[i][k];
               exact[i] = 0;
            }
            ++n;
         } else if ( i+k-c >=0 && i+k-c <etot &&           /* simple average */
                     i+k-c != 1 && i+k-c != etot-2)  {    /* because no data */
            psum += lnP[i+k-c];
            norm += 1.0;
            exact[i] = 0;
         }
         
      }
      if (read_lnW && T[i][class] > 0.0 && !exact[i] ) {
            psum += (R[i][class] + lnw[i])/sig[i][class];
            norm += 1.0/sig[i][class];
      }
                    
      if (iter == 1) {
         lnP[i] = psum/norm;
      } else {
         lnP[i] = 0.9 * (psum/norm)  + 0.1 * lnP[i];
      }
   }

   for(i = 0; i < etot/2; ++i) {
      psum = 0.5 * (lnP[i] + lnP[etot-i-1]);
      lnP[i] = lnP[etot-i-1] = psum;
   }

   if (read_n) {                                        /* normalized to Q^N */
      max = -1.0e20;
      for(i = 0; i < etot; ++i) {
         if (lnP[i] > max) max = lnP[i];
      }
   
      norm = 0.0;
      for(i = 0; i < etot; ++i) {
         norm += exp( (lnP[i] - max) );
      }
      a = Nv * log( (double) Qv) - max;

      
      r = 2.0*(1.0 + Nv)*exp(-a-max);
      del = (1.0 - r)/(norm * exp(-a) - r);
      assert(del > 0.0);
      if (del > 0.0) {
         del = log(del);
      } else {
         del = 0.0;
      }
      for(i = 3; i < etot-3; ++i) {
         lnP[i] += del;
      } 

   }

   F_old = F;
   F = 0.0;
   for(i = 0; i < etot; ++i) { 
      for(k = 0; k < class; ++k) {
         if(k == c) continue;
         if ( R[i][k] < 1.0e25 && fabs(sig[i][k]) > 1.0e-14) { 
            del = -lnP[i] + lnP[i+k-c] + R[i][k];
            if(fabs(del) > 1e10)
             printf("%g, %d, %d, %g, %g, %g\n",
                   del, i, k, lnP[i], lnP[i+k-c], R[i][k]);
            F += del*del/sig[i][k];
            assert( i+k-c >= 0 && i+k-c < etot);
         }
      }
      if (read_lnW && T[i][class] > 0.0 && !exact[i]) {
            del = - lnP[i] + R[i][class] + lnw[i];
            F += del*del/sig[i][class];
      }
   }

   if (read_lnW) {
      neqn = class*etot;
   } else {
      neqn = (class-1) * etot;
   }
   dF = F_old - F;
   if (F != 0.0) dF = dF/F;
/*
   if (iter % etot*etot == 0 ) printf("iter = %d, F/per eq = %.10g, dF/F = %g\n", 
                                 iter, F/neqn, dF);
*/
   if ( iter < 10*etot*etot && fabs(dF) > 1.0e-16) goto Start;

/*
   printf("beta = %g, dF = %g\n", beta, dF);

   printf("%d %d %d\n", Dv, Qv, Lv);
   for(i = 0; i < etot; ++i) {
      printf("%.15g  %.15g\n", (double) i + 0.5*Shift_up, lnP[i] - beta * i);
   }
*/

}

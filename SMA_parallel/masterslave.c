#include "mpi.h"
#include <stdio.h>

#define WORKTAG 1
#define DIETAG 2

static void master(void);
static void slave(void);
static int get_next_work_item(void);


int main(int argc, char **argv)
{
  int myid;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  if (myid == 0) {
    master();
  } else {
    slave();
  }

  MPI_Finalize();
  return 0;
}


int njobs;

static void master(void)
{
  int np, rank, rank_max;
  int work;
  double result;
  MPI_Status status;

  printf("enter number of jobs njobs\n");
  scanf("%d", &njobs);
  MPI_Comm_size(MPI_COMM_WORLD, &np);

  /* send to each slave one job first */
  rank = 1;
  while( (work = get_next_work_item()) != -1 && rank < np) {
    MPI_Send(&work, 1, MPI_INT, rank++, WORKTAG, MPI_COMM_WORLD);
  }
  rank_max = rank;

  /* wait to receive and send new job */
  while (work != -1) {

    MPI_Recv(&result, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);  

    MPI_Send(&work, 1, MPI_INT, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);

    work = get_next_work_item();
  }

  /* There's no more work to be done, so receive all the outstanding
     results from the slaves. */

  for (rank = 1; rank < rank_max; ++rank) {
    MPI_Recv(&result, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
             MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  }

  /* Tell all the slaves to exit by sending an message with the
     DIETAG. */

  for (rank = 1; rank < np; ++rank) {
    MPI_Send(&work, 1, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);
  }
}


static void slave(void)
{
  int work, myid;
  double result;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  while (1) {

    printf("slave %d prepare to receive\n", myid);
    MPI_Recv(&work, 1, MPI_INT, 0, MPI_ANY_TAG,
             MPI_COMM_WORLD, &status);
    printf(" work %d\n", work);
    fflush(stdout);

    /* Check the tag of the received message. */
    if (status.MPI_TAG == DIETAG) {
      printf("slave %d dies\n", myid);
      fflush(stdout);
      return;
    }

    /* Do the work, sleep work second  */
    sleep(rand()%4);    

    result = work;
    MPI_Send(&result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    printf("slave %d finished work %d\n", myid, work);
    fflush(stdout);
  }
}


static int get_next_work_item(void)
{
   static int ncalls = 0;
   ++ncalls;

   if (ncalls <= njobs) {
     return ncalls;
   } else {
     return -1;
   }
}

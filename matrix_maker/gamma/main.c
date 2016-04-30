#include <stdio.h>
#include <stdlib.h>
/* #include "mmio.h" */
/* #define N 10 */
/* #define GAMMA 1.5 */

int main(int argc, char const* argv[])
{
  if(argc!=3){
    printf("Usage: ./a.out gamma size\n");
    exit(1);
  }
  double GAMMA = 1.0;
  long int N =10;

  GAMMA = atof(argv[1]);
  N = atoi(argv[2]);
  printf("gamma=%g\n",GAMMA);
  printf("size =%ld\n",N);

  //double a[N][N];
  double *a;
  a=(double *)malloc(sizeof(double)*N*N);
  long int i, j;

  int ndata = 0;
  int nozero = 0;

  FILE *out;

  /* MM_typecode matcode; */
  
  if((out = fopen("output.txt", "w")) == NULL){
    printf("file open error!!\n");
    exit(1);
  }
  ndata = N;
  
  for(i = 0;i<N;i++){
    for(j = 0;j<N;j++){
      //a[i][j] = 0.0;
      a[i*N+j] = 0.0;
    }
  }

  for(i = 0;i<N;i++){
    for(j = 0;j<N;j++){
      //a[i][i] = 2.0;
      a[i*N+i]=GAMMA;
      if(j == i+1)
        //a[i][j] = 1.0;
       a[i*N+j]=-1.0;
      if(j == i-1)
        //a[i][j] = GAMMA;
       a[i*N+j]=-1.0; 
   }
  }

  for(i = 0;i<N;i++){
    for(j = 0;j<N;j++){
      //if(a[i][j]!=0.0)
       if(a[i*N+j]!=0.0)
        nozero++;
    }
  }
/*  for(i = 0;i<N;i++){ */
/*    for(j = 0;j<N;j++){ */
/*      printf("%3.1f ", a[i*N+j]); */
/*    } */
/*    putchar('\n'); */
/*  } */

  fprintf(out, "%%%%MatrixMarket matrix coordinate real general\n");
  fprintf(out, "%d %d %d\n", ndata, ndata, nozero);
  for(j = 0;j<N;j++){
    for(i = 0;i<N;i++){
      //if(a[i][j]!=0.0)
      if(a[i*N+j]!=0.0){
        /* printf("%d %d %3.1f\n",i, j,  a[i*N+j]); */
        //fprintf(out, "%d %d %.16e\n", i+1, j+1, a[i][j]);
        fprintf(out, "%ld %ld %.16e\n", i+1, j+1, a[i*N+j]);
    } 
   }
  }

  fclose(out);
  free(a);
  return 0;















}

#include <stdio.h>
#include <stdlib.h>
/* #include "mmio.h" */
#define N 1000
#define GAMMA 1.5

int main(int argc, char const* argv[])
{
  //double a[N][N];
  double *a;
  a=(double *)malloc(sizeof(double)*N*N);
  int i, j;

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
      a[i*N+i]=2.0;
      if(j == i+1)
        //a[i][j] = 1.0;
       a[i*N+j]=1.0;
      if(j == i-2)
        //a[i][j] = GAMMA;
       a[i*N+j]=GAMMA; 
   }
  }

  for(i = 0;i<N;i++){
    for(j = 0;j<N;j++){
      //if(a[i][j]!=0.0)
       if(a[i*N+j]!=0.0)
        nozero++;
    }
  }
printf("make Toplitz matrix in %d size\n",N);
//  for(i = 0;i<N;i++){
//    for(j = 0;j<N;j++){
      //printf("%3.1f ", a[i][j]);
//      printf("%3.1f ", a[i*N+j]);
//    }
//    putchar('\n');
//  }

  fprintf(out, "%%%%MatrixMarket matrix coordinate real general\n");
  fprintf(out, "%d %d %d\n", ndata, ndata, nozero);
  for(j = 0;j<N;j++){
    for(i = 0;i<N;i++){
      //if(a[i][j]!=0.0)
      if(a[i*N+j]!=0.0){
        /* printf("%d %d %3.1f\n",i, j,  a[i][j]); */
        //fprintf(out, "%d %d %.16e\n", i+1, j+1, a[i][j]);
        fprintf(out, "%d %d %.16e\n", i+1, j+1, a[i*N+j]);
    } 
   }
  }

  fclose(out);
  free(a);
  return 0;
}

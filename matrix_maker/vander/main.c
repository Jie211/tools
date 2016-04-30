#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const* argv[])
{
  if(argc!=2){
    printf("Usage: ./a.out size\n");
    exit(1);
  }
  long int N =10;

  N = atoi(argv[1]);
  printf("size =%ld\n",N);

  double *a;
  a=(double *)malloc(sizeof(double)*N);
  long int i, j;


  FILE *out;

  if((out = fopen("output.txt", "w")) == NULL){
    printf("file open error!!\n");
    exit(1);
  }
  
  for(i = 0;i<N;i++){
      a[i] = 1.0+0.5*i;
  }
  for(i=0;i<N;i++){
  printf("%f ", a[i]);
  }
  putchar('\n');
  /* fprintf(out, "%%%%MatrixMarket matrix coordinate real general\n"); */
  /* fprintf(out, "%d %d %d\n", ndata, ndata, nozero); */

  fclose(out);
  free(a);
  return 0;















}

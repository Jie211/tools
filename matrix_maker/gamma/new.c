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
  double side = -1.0;
  long int N =10;

  GAMMA = atof(argv[1]);
  N = atoi(argv[2]);

  long int nozero = 3*(N-2)+4;
  long int line=0;
  long int i,j;

  FILE *out;

  if((out = fopen("output.txt", "w")) == NULL){
    printf("file open error!!\n");
    exit(1);
  }
  
  fprintf(out, "%%%%MatrixMarket matrix coordinate real general\n");

  fprintf(out, "%ld %ld %ld\n", N, N, nozero);

  for(j=0;j<N;j++){
    long int index=0;

    if(line==0){
      fprintf(out, "1 1 %.16e\n", GAMMA);
      fprintf(out, "2 1 %.16e\n", side);
      line++;
      continue;
    }else if(line==(N-1)){
      fprintf(out, "%ld %ld %.16e\n", N-1, N, side);
      fprintf(out, "%ld %ld %.16e\n", N, N, GAMMA);
      line++;
      continue;
    }

    /* for(i=0;i<3;i++){ */
    fprintf(out, "%ld %ld %.16e\n", (j-1)+1, j+1, side);
    fprintf(out, "%ld %ld %.16e\n", (j-1)+2, j+1, GAMMA);
    fprintf(out, "%ld %ld %.16e\n", (j-1)+3, j+1, side);
    /* index++; */
    /* } */
    line++;
  }

  fclose(out);
  return 0;















}

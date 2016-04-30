#include <stdio.h>
#include <stdlib.h>
/* #include "mmio.h" */
#define N 100

int main(int argc, char const* argv[])
{
  double a[N][N];
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
      a[i][j] = 0.0;
    }
  }
  
  for(i = 0;i<N;i++){
    for(j = 0;j<N;j++){
      a[i][i] = 4.0;
      if(j == i+1)
        a[i][j] = 1.0;
      if(j == i-1)
        a[i][j] = 1.0;
    }
  }
  
  for(i = 0;i<N;i++){
    for(j = 0;j<N;j++){
      if(a[i][j]!=0.0)
        if(j>=i)
          nozero++;
    }
  }

  /* for(i = 0;i<N;i++){ */
  /*   for(j = 0;j<N;j++){ */
  /*     printf("%3.1f\t", a[i][j]); */
  /*   } */
  /*   putchar('\n'); */
  /* } */

  /* mm_initialize_typecode(&matcode); */
  /* mm_set_matrix(&matcode); */
  /* mm_set_sparse(&matcode); */
  /* mm_set_coordinate(&matcode); */
  /* mm_set_dense(&matcode); */
  /* mm_set_real(&matcode); */
  /* mm_set_symmetric(&matcode); */

  /* mm_write_banner(out, matcode); */
  
  /* mm_write_mtx_crd_size(out, ndata, ndata, nozero); */
  fprintf(out, "%%%%MatrixMarket matrix coordinate real symmetric\n");
  fprintf(out, "%d %d %d\n", ndata, ndata, nozero);
  for(j = 0;j<N;j++){
    for(i = 0;i<N;i++){
      if(a[j][i]!=0.0)
        if(i>=j)
        /* printf("%d %d %3.1f\n",i, j,  a[i][j]); */
          fprintf(out, "%d %d %.16e\n", i+1, j+1, a[i][j]);
    }
  }

  fclose(out);
  return 0;
}

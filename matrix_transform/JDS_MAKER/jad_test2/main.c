#include "header.h"

int comp(const void *x, const void *y)
{
  if(x == y) return 0;
  if(x == 0) return  -1;
  if(y == 0) return 1;

  const mat *ptr1 = (const mat*)x, *ptr2 = (const mat*)y;

  if(ptr1->nz > ptr2->nz) return  -1;
  if(ptr1->nz < ptr2->nz) return 1;
  
  return 0;
}

int comp2(const void *x, const void *y)
{
  if(x == y) return 0;
  if(x == 0) return  -1;
  if(y == 0) return 1;

  const mat *ptr1 = (const mat*)x, *ptr2 = (const mat*)y;

  if(ptr1->col_n < ptr2->col_n) return  -1;
  if(ptr1->col_n > ptr2->col_n) return 1;
  
  if(ptr1->row_n < ptr2->row_n) return  -1;
  if(ptr1->row_n > ptr2->row_n) return 1;

  return 0;
}

int main(int argc, char const* argv[])
{
  int N;
  int M;
  int MAX;
  double *b;
  double *x;

  mat *matrix;
  int *ptr;
  int *nz_count;
  int *n_ptr;
  int *n_nz;
  int *n_perm;
  int *nn_ptr;
  int max_nz = 0;
  double *x_n;
  double *b_n;

 FILE *output1;
 FILE *output2;
 FILE *output3;
 FILE *output4;


  
  
  


  if(argc!=4)
  {
    printf("Usage error\n");
    printf("%s colval ptr bx maxloop thread\n", argv[0]);
    exit(0);
  }
#ifdef EBUG
  printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
  printf("get headpart and check headerror\n");
#endif
  gethead(argv[1],argv[2],argv[3],&N,&M); 
#ifdef EBUG
  printf("get N = %d M = %d\n", N, M);
#endif
  b=(double *)malloc(sizeof(double)*N);
  x=(double *)malloc(sizeof(double)*N);
  if(!b || !x)
  {
    printf("malloc b x error");
    if(b)
      free(b);
    if(x)
      free(x);
    exit(1);
  }


  matrix = (mat *)malloc(sizeof(mat)*M);
  ptr=(int *)malloc(sizeof(int)*(N+1));
  if(!matrix ||  !ptr)
  {
    printf("malloc val col ptr error");
    if(b)
      free(b);
    if(x)
      free(x);
    if(matrix)
      free(matrix);
    if(ptr)
      free(ptr);
    exit(1);
  }
#ifdef EBUG  
  printf("read data\n");
#endif
  getdata(argv[1],argv[2],argv[3],ptr,matrix,b,x,N,M);
#ifdef EBUG
  printf("read data over\n");
  printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
#endif
  MAX = N*2.5;
  printf("process\n");

  
  /* printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"); */
  /* for(int i = 0;i<N+1;i++){ */
  /*   printf("[%d] ptr %d \n",i, ptr[i]); */
  /* } */

  printf("creat nz perm\n");
  nz_count = (int *)malloc(sizeof(int)*N);
  int i_count = 0;
  for(int i = 0;i<N;i++){
    int count = 0;
    for(int j = ptr[i];j<ptr[i+1];j++){
     count++;
    }
    nz_count[i_count] = count;
    i_count++;
  }
  
  n_perm= (int *)malloc(sizeof(int)*N);
  int perm_count = 0;
  i_count = 0;
  for(int i = 0;i<N;i++){
    for(int j = ptr[i];j<ptr[i+1];j++){
     matrix[j].nz = nz_count[i_count];
     matrix[j].perm = perm_count;
     if(matrix[j].nz > max_nz){
      max_nz = matrix[j].nz;
     }
    }
    perm_count++;
    i_count++;
  }
  /* printf("++++++ MAX  = %d\n", max_nz); */

/* printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"); */
/*   for(int i = 0;i<N;i++){ */
/*     for(int j = ptr[i];j<ptr[i+1];j++){ */
/*       printf("perm %d, row %d, col %d, val %3.1f, nz = %d\n",matrix[j].perm, matrix[j].row, matrix[j].col, matrix[j].val, matrix[j].nz); */
/*     } */
/*     printf("_____________________________\n"); */
/*   } */
  printf("sort 1\n");
  qsort(matrix, M, sizeof(mat), comp);
/* printf("~~~~~~~~~~~sort1~~~~~~~~~~~~\n"); */
/*   for(int i = 0;i<N;i++){ */
/*     for(int j = ptr[i];j<ptr[i+1];j++){ */
/*       printf("perm %d, row %d, col %d, val %3.1f, nz = %d \n",matrix[j].perm, matrix[j].row, matrix[j].col, matrix[j].val, matrix[j].nz); */
/*     } */
/*     printf("_____________________________\n"); */
/*   } */

  printf("creat n_ptr\n");
  n_ptr = (int *)malloc(sizeof(int)*(N+1));
  int ptr_count = 0;
  i_count = 1;
  n_ptr[0] = 0;
  int check = matrix[0].row;
  for(int i = 0;i<N;i++){
    for(int j = ptr[i];j<ptr[i+1];j++){
      if(check != matrix[j].row){
        check  =  matrix[j].row;
        n_ptr[i_count] = ptr_count;
        i_count++;
      }
      ptr_count++;
    }
  }
  n_ptr[N] = M; 

  printf("creat n_nz n_perm\n");
  n_nz  = (int *)malloc(sizeof(int)*N);
  i_count = 0;
  for(int i = 0;i<N;i++){
    for(int j = n_ptr[i];j<n_ptr[i+1];j++){
        n_nz[i_count] = matrix[j].nz;
        n_perm[i_count] = matrix[j].perm;
    }
    i_count++;
  }
/* printf("~~~~~~~~~~~fix n_ptr~~~~~~~~~~~~\n"); */
/*   for(int i = 0;i<N;i++){ */
/*     for(int j = n_ptr[i];j<n_ptr[i+1];j++){ */
/*       printf("perm %d, row %d, col %d, val %3.1f, nz = %d \n",matrix[j].perm, matrix[j].row, matrix[j].col, matrix[j].val, matrix[j].nz); */
/*     } */
/*     printf("_____________________________\n"); */
/*   } */

  printf("creat col_n row_n\n");
  int col_count = 0;
  int row_count = 0;
  for(int i = 0;i<N;i++){
    for(int j = n_ptr[i];j<n_ptr[i+1];j++){
      matrix[j].col_n = col_count;
      matrix[j].row_n = row_count;
      col_count++;
    }
    col_count = 0;
    row_count++;
  }

  printf("sort 2\n");
  qsort(matrix, M, sizeof(mat), comp2);
  /* printf("~~~~~~~~~~~sort2~~~~~~~~~~~~\n"); */
  /* for(int i = 0;i<N;i++){ */
  /*   for(int j = n_ptr[i];j<n_ptr[i+1];j++){ */
  /*     printf("perm %d, row %d, col %d, row_n = %d, col_n = %d, val %3.1f, nz = %d \n",matrix[j].perm, matrix[j].row, matrix[j].col, matrix[j].row_n, matrix[j].col_n, matrix[j].val, matrix[j].nz); */
  /*   } */
  /*   printf("_____________________________\n"); */
  /* } */

  /* int nn_ptr[max_nz+1]; */
  printf("creat jd\n");
  nn_ptr= (int *)malloc(sizeof(int)*(max_nz+1));
  int nn_count = 0;
  i_count = 1;
  nn_ptr[0] = 0;
  int nn_check = matrix[0].col_n;
  /* printf("first  = %d\n", nn_check); */

  for(int i = 0;i<M;i++){
    if(nn_check != matrix[i].col_n){
      nn_check = matrix[i].col_n;
      nn_ptr[i_count] = nn_count;
      i_count++;
      /* printf("i = %d check = %d col_n = %d\n", i, nn_check,  matrix[i].col_n); */
    }
    nn_count++;
  }
  nn_ptr[max_nz] = M;

  printf("create x b\n");
  x_n = (double *)malloc(sizeof(double)*N);
  b_n = (double *)malloc(sizeof(double)*N);

  for(int i = 0;i<N;i++){
    x_n[i] = 1.0;
    b_n[i] = 0.0;
  }

  /* printf("x = \n"); */
  /* for(int i = 0;i<N;i++){ */
  /*   printf("%3.1f ", x[i]); */
  /* } */
  /* putchar('\n'); */

  int xvm = 0.0;
  for(int i = 0;i<n_nz[0];i++){
    xvm = 0.0;
    for(int j = nn_ptr[i];j<nn_ptr[i+1];j++){
      b_n[n_perm[j - nn_ptr[i]]] += matrix[j].val * x_n[matrix[j].col]; 
    }
  }
  
/* printf("~~~~~~~~~~~OUT~~~~~~~~~~~~\n"); */
/*   for(int i = 0;i<n_nz[0];i++){ */
/*     for(int j = nn_ptr[i];j<nn_ptr[i+1];j++){ */
/*       printf("perm %d, row %d, col %d, row_n = %d, col_n = %d, val %3.1f, nz = %d \n",matrix[j].perm, matrix[j].row, matrix[j].col, matrix[j].row_n, matrix[j].col_n, matrix[j].val, matrix[j].nz); */
/*     } */
/*     printf("_____________________________\n"); */
/*   } */
  /* printf("nz = \n"); */
  /* for(int i = 0;i<N;i++){ */
  /*   printf("%d ", n_nz[i]); */
  /* } */
  /* putchar('\n'); */
  /*  */
  /* printf("perm = \n"); */
  /* for(int i = 0;i<N;i++){ */
  /*   printf("%d ", n_perm[i]); */
  /* } */
  /* putchar('\n'); */
  /*  */
  /* printf("jd = \n"); */
  /* for(int i = 0;i<max_nz+1;i++){ */
  /*   printf("%d ", nn_ptr[i]); */
  /* } */
  /* putchar('\n'); */
  /*  */
  /* printf("mat = \n"); */
  /* for(int i = 0;i<M;i++){ */
  /*   printf("%3.1f ", matrix[i].val); */
  /* } */
  /* putchar('\n'); */
  /*  */
  /* printf("col = \n"); */
  /* for(int i = 0;i<M;i++){ */
  /*   printf("%d ", matrix[i].col); */
  /* } */
  /* putchar('\n'); */
  /*  */
  /*    */
  /* printf("b = \n"); */
  /* for(int i = 0;i<N;i++){ */
  /*   printf("%3.1f ", b[i]); */
  /* } */
  /* putchar('\n'); */

  if((output1 = fopen("perm_nz.txt", "w")) == NULL)
      exit(1); 
  if((output2 = fopen("mat_col.txt", "w")) == NULL)
      exit(1);
  if((output3 = fopen("jd.txt", "w")) == NULL)
      exit(1); 
  
  if((output4 = fopen("bx.txt", "w")) == NULL)
      exit(1); 

  printf("output perm_nz \n");
  fprintf(output1, "%d %d %d\n", N, N, M);
  for(int i = 0;i<N;i++){
    fprintf(output1, "%d %d\n", n_perm[i], n_nz[i]);
  }

  printf("output mat_col \n");
  fprintf(output2, "%d %d %d\n", N, N, M);
  for(int i = 0;i<M;i++){
    fprintf(output2, "%.16e %d\n", matrix[i].val, matrix[i].col);
  }

  printf("output jd \n");
  fprintf(output3, "%d %d %d\n", N, N, M);
  for(int i = 0;i<max_nz+1;i++){
    fprintf(output3, "%d\n", nn_ptr[i]);
  }
  
  printf("output bx \n");
  fprintf(output4, "%d %d %d\n", N, N, M);
  for(int i = 0;i<N;i++){
    fprintf(output4, "%.16e %.16e\n", b_n[i], x_n[i]);
  }

  fclose(output1);
  fclose(output2);
  fclose(output3);
  fclose(output4);

  free(b);
  free(x);
  free(matrix);
  free(ptr);
  free(n_ptr);
  free(n_nz);
  free(n_perm);
  free(nn_ptr);
  free(x_n);
  free(b_n);


  return 0;
}

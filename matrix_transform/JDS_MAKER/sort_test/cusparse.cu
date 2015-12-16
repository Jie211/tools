#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda_runtime.h>
#include <cusparse.h>
#include "mmio.h"
/* #include <cusparse_v2.h> */

#define CLEANUP(s)  \
do{                 \
  printf("%s\n", s);\
  if(f) fclose(f);\
  if(cooRow) free(cooRow);\
  if(cooCol) free(cooCol);\
  if(cooVal) free(cooVal);\
  if(matrix) free(matrix);\
  if(csrptr)  free(csrptr);\
  if(d_cooCol) cudaFree(d_cooCol);\
  if(d_cooRow) cudaFree(d_cooRow);\
  if(d_cooVal) cudaFree(d_cooVal);\
  if(descr) cusparseDestroyMatDescr(descr);\
  if(handle) cusparseDestroy(handle);\
  if(output1) fclose(output1);\
  if(output2) fclose(output2);\
  if(output3) fclose(output3);\
  if(x) free(x);\
  if(b) free(b);\
  cudaDeviceReset();        \
  fflush(stdout);           \
}while(0)

typedef struct{
  int row;
  int col;
  double val;
}mat;

int comp(const void *x, const void *y)
{
  if(x == y) return 0;
  if(x == 0) return -1;
  if(y == 0) return 1;

  const mat *ptr1 = (const mat*)x, *ptr2 = (const mat*)y;

  if(ptr1->row < ptr2->row) return -1;
  if(ptr1->row > ptr2->row) return 1;

  if(ptr1->col < ptr2->col) return -1;
  if(ptr1->col > ptr2->col) return 1;

  return 0;
}

int main(int argc, char *argv[]){
  srand((unsigned)time(NULL));
  int *cooRow=0;
  int *cooCol=0;
  double *cooVal=0;
  mat *matrix;
  
  int *d_cooRow = 0;
  int *d_cooCol = 0;
  double *d_cooVal = 0;
  
  int N, M;
  int NNZ;

  cudaError_t stat1, stat2, stat3;
  cusparseHandle_t handle = 0;
  cusparseMatDescr_t descr = 0;

  int *d_csrptr = 0;
  int *csrptr = 0;

  int size_read;
  MM_typecode type_read;
  FILE *f;

  FILE *output1;
  FILE *output2;

  double getMAX = 0;
  double getMIN = 0;

  FILE *output3;
  double *x = 0;
  double *b = 0;
  double xvm;

if(argc<2)
  {
    fprintf(stderr, "Usage: %s [matrix-market-filename]\n", argv[0]);
    exit(0);
  }else
  {
    if((f = fopen(argv[1], "r")) == NULL)
      exit(1);
  }

  if(mm_read_banner(f, &type_read)!= 0)
  {
    printf("could not process matrix market banner.\n");
    exit(1);
  }

  if(mm_is_complex(type_read) && mm_is_matrix(type_read) && mm_is_sparse(type_read))
  {
    printf("this app does not support ");
    printf("Market market type '[%s]\n", mm_typecode_to_str(type_read));
  }

  if((size_read = mm_read_mtx_crd_size(f, &M, &N, &NNZ))!= 0)
    exit(1);

  printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  mm_write_mtx_crd_size(stdout, M, N, NNZ);
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");

  if(M!=N){
    CLEANUP("M!=N size error");
    return  -1;
  }
  printf("N=%d NNZ=%d\n",N,NNZ);
  int NNZ_old;
  NNZ_old = NNZ;
  NNZ = NNZ*2 - N;
  printf("NNZ %d was fix to %d\n", NNZ_old, NNZ);
  cooRow=(int *)malloc(sizeof(cooRow[0])*NNZ);
  cooCol=(int *)malloc(sizeof(cooCol[0])*NNZ);
  cooVal=(double *)malloc(sizeof(cooVal[0])*NNZ);
  matrix = (mat *)malloc(sizeof(mat)*NNZ);


  if((!cooRow)||(!cooCol)||(!cooVal)){
    CLEANUP("Host malloc matrix coo failed");
    return  -1;
  }
  if((!matrix)){
    CLEANUP("Host malloc matrix failed");
    exit(1);
  }
  int r_i;
  int r_j;
  double r_val;
  printf("Input data_______\n");
  for(int i = 0;i<NNZ_old;i++)
  {
    fscanf(f, "%d %d %lg\n", &r_i, &r_j, &r_val);
    /* cooRow[i] = r_i - 1; */
    /* cooCol[i] = r_j - 1; */
    /* cooVal[i] = r_val; */
    matrix[i].row = r_i - 1;
    matrix[i].col = r_j - 1;
    matrix[i].val = r_val;
  }
  int NNZcount = 0;
  for(int i = 0;i<NNZ_old;i++)
  {
    if(matrix[i].row != matrix[i].col)
    {
      /* cooRow[NNZ_old+NNZcount] = cooCol[i]; */
      /* cooCol[NNZ_old+NNZcount] = cooRow[i]; */
      /* cooVal[NNZ_old+NNZcount] = cooVal[i]; */
      matrix[NNZ_old+NNZcount].row = matrix[i].col;
      matrix[NNZ_old+NNZcount].col = matrix[i].row;
      matrix[NNZ_old+NNZcount].val = matrix[i].val;
      NNZcount++;
    }
  }


  /* for(int i = 0;i<NNZ;i++){ */
  /*   printf("index[%d]->", i); */
  /*   printf("Row=%d ", matrix[i].row); */
  /*   printf("Col=%d ", matrix[i].col); */
  /*   printf("Val=%f\n", matrix[i].val); */
  /* } */
  printf("Input over\n");
  printf("start sort_________\n");

  /* sort(cooRow, cooCol, cooVal, NNZ); */
  qsort(matrix, NNZ, sizeof(mat), comp);
  /* for(int i = 0;i<NNZ;i++){ */
  /*   printf("index[%d]->", i); */
  /*   printf("Row=%d ", cooRow[i]); */
  /*   printf("Col=%d ", cooCol[i]); */
  /*   printf("Val=%f\n", cooVal[i]); */
  /* } */
  printf("sort over\n");
  printf("getMAX getMIN\n");
  getMAX = getMIN = cooVal[0];
  for(int i = 0;i<NNZ;i++)
  {
    /* if(cooVal[i]>=getMAX){ */
    if(matrix[i].val>=getMAX){
      /* getMAX = cooVal[i]; */
      getMAX = matrix[i].val;
    }
    /* if(cooVal[i]<=getMIN){ */
    if(matrix[i].val<=getMIN){
      /* getMIN = cooVal[i]; */
      getMIN = matrix[i].val;
    }
  }
 
  for(int i = 0;i<NNZ;i++)
  {
    cooRow[i] = matrix[i].row;
    cooCol[i] = matrix[i].col;
    cooVal[i] = matrix[i].val;
  }

  printf("MAX = %f\nMIN = %f\n", getMAX, getMIN);
  printf("coo device malloc\n");
  stat1 = cudaMalloc((void**)&d_cooRow, sizeof(d_cooRow)*NNZ);
  stat2 = cudaMalloc((void**)&d_cooCol, sizeof(d_cooCol)*NNZ);
  stat3 = cudaMalloc((void**)&d_cooVal, sizeof(d_cooVal)*NNZ);
  if((stat1!= cudaSuccess)||
      (stat2!= cudaSuccess)||
      (stat3!= cudaSuccess)){
    CLEANUP("Device malloc failed");
    return  -1;
  }
  printf("memcpy coo to device\n");
  stat1 = cudaMemcpy(d_cooRow, cooRow, (size_t)(sizeof(d_cooRow[0])*NNZ), cudaMemcpyHostToDevice);
  stat2 = cudaMemcpy(d_cooCol, cooCol, (size_t)(sizeof(d_cooCol[0])*NNZ), cudaMemcpyHostToDevice);
  stat3 = cudaMemcpy(d_cooVal, cooVal, (size_t)(sizeof(d_cooVal[0])*NNZ), cudaMemcpyHostToDevice);
  if((stat1!= cudaSuccess)||
      (stat2!= cudaSuccess)||
      (stat3!= cudaSuccess)){
    CLEANUP("Memcpy host 2 device failed");
    return  -1;
  }
  cusparseStatus_t custat;
  printf("create handle and descriptor\n"); 
  custat = cusparseCreate(&handle);
  if(custat!= CUSPARSE_STATUS_SUCCESS){
    CLEANUP("CUSPARSE lib initialization failed");
    return 1;
  }
  custat = cusparseCreateMatDescr(&descr);
  if(custat!= CUSPARSE_STATUS_SUCCESS){
    CLEANUP("Matrix descriptor initialization failed");
    return 1;
  }
  printf("set descriptor type and indexbase\n");
  cusparseSetMatType(descr, CUSPARSE_MATRIX_TYPE_GENERAL);
  cusparseSetMatIndexBase(descr, CUSPARSE_INDEX_BASE_ZERO);

  csrptr = (int *)malloc(sizeof(csrptr[0])*(N + 1));
  if(!csrptr){
    CLEANUP("csrptr malloc failed");
    return  -1;
  }
  stat1 =  cudaMalloc((void**)&d_csrptr, sizeof(d_csrptr[0])*(N+1));
  if(stat1!=cudaSuccess){
    CLEANUP("Device malloc failed");
    return 1;
  }
  printf("conversion ptr vec___________\n");
  custat = cusparseXcoo2csr(handle, d_cooRow, NNZ, N, d_csrptr, CUSPARSE_INDEX_BASE_ZERO);
  if(custat!=CUSPARSE_STATUS_SUCCESS){
    CLEANUP("conversion coo 2 csr failed");
    return 1;
  }
  stat1 = cudaMemcpy(csrptr, d_csrptr, (size_t)(sizeof(d_csrptr[0])*(N+1)), cudaMemcpyDeviceToHost);
  if(stat1!=cudaSuccess){
    CLEANUP("Memcpy d_csrptr  ->csrptr failed ");
    return 1;
  }
  printf("conversion done\n");
  /* for(int i = 0;i<N +1;i++){ */
  /*   printf("csrptr[%d]=%d\n", i, csrptr[i]); */
  /* } */
  printf("create x random by %f~%f\n", getMIN, getMAX);
  x = (double *)malloc(sizeof(x[0])*N);
  b = (double *)malloc(sizeof(b[0])*N);
  for(int i = 0;i<N;i++)
  {
    /* if(fabs(getMAX)>=RAND_MAX || fabs(getMIN)>=RAND_MAX) */
    /* { */
    /*   x[i] = rand(); */
    /* }else{ */
    /*   x[i] = (double)(rand()%((int)getMAX*10 - (int)getMIN*10 + 1)+(int)getMIN*10)/10.0; */
    /* } */
    x[i] = 1.0;
  }
  printf("create b by x and Matrix\n");
  for(int i = 0;i<N;i++)
  {
    xvm = 0.0;
    for(int j = csrptr[i];j<csrptr[i+1];j++)
    {
      xvm+=cooVal[j]*x[cooCol[j]];
    }
    b[i] = xvm;
  }
  printf("ready to output________\n");
  if((output1 = fopen("./ColVal.txt", "w")) == NULL)
      exit(1); 
  if((output2 = fopen("./Ptr.txt", "w")) == NULL)
      exit(1);
  if((output3 = fopen("./bx.txt", "w")) == NULL)
      exit(1); 
  
  printf("output col val\n");
  fprintf(output1, "%d %d %d\n", N, M, NNZ);
  for(int i = 0;i<NNZ;i++)
  {
    fprintf(output1, "%d %d %.16e\n", cooRow[i], cooCol[i], cooVal[i]);
  }
  printf("output ptr\n");
  fprintf(output2, "%d %d %d\n", N, M, NNZ);
  for(int i = 0;i<N+1; i++)
  {
    fprintf(output2, "%d\n", csrptr[i]);
  }
  printf("output b x\n");
  fprintf(output3, "%d %d %d\n", N, M, NNZ);
  for(int i = 0;i<N;i++)
  {
    fprintf(output3, "%.16e %.16e\n", b[i], x[i]);
  }

  printf("output over \n");

  CLEANUP("Program safety end\n");
  return 0;
}

#include "header.h"

#define CLEANUP(s) \
  do{               \
    printf("%s\n",s);\
    if(b) free(b);\
    if(x) free(x);\
    if(jd) free(jd);\
    if(perm) free(perm);\
    if(col) free(col);\
    if(nz) free(nz);\
    if(val) free(val);\
    cudaDeviceReset();\
    fflush(stdout);\
  }while(0)\

int main(int argc, char const* argv[])
{
  int N;
  int M;
  int MAXNZ;

  double *b;
  double *x;
  int *jd;
  int *perm;
  int *col;
  int *nz;
  double *val;
  
  char copy[256];
  const char *hoge;
  const char *hoge2;
  const char *hoge3;

  if(argc!=5)
  {
    printf("Usage error\n");
    printf("%s mat_col perm_nz jd  bx \n", argv[0]);
    exit(0);
  }

#ifdef EBUG
  printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
  printf("get headpart and check headerror\n");
#endif
  gethead_jad(argv[1],argv[2],argv[3],argv[4], &N,&M); 
#ifdef EBUG
  printf("get N = %d M = %d\n", N, M);
  printf("~~~~~~~ header ok ~~~~~~~~\n");
#endif

  strcpy(copy, argv[1]);
  hoge = strtok(copy, "/");
  hoge2 = strtok(NULL, "/");
  hoge3 = strtok(NULL, "/");
  printf("Matrix = %s\n", hoge3);
  strcpy(copy, hoge2);
  strcpy(copy, hoge);

  b=(double *)malloc(sizeof(double)*N);
  x=(double *)malloc(sizeof(double)*N);

  if(!b || !x)
  {
    CLEANUP("malloc b x error");
  }

  getmaxnz_jad(argv[2],&MAXNZ);
  val=(double *)malloc(sizeof(double)*M);
  col=(int *)malloc(sizeof(int)*M);
  perm = (int *)malloc(sizeof(int)*N);
  nz = (int *)malloc(sizeof(int)*N);
  jd = (int *)malloc(sizeof(int)*(MAXNZ+1));

  if(!val || !col || !perm || !nz || !jd)
  {
    CLEANUP("malloc val col perm nz jd");
  }

#ifdef EBUG  
  printf("read data\n");
#endif
 getdata_jad(argv[1], argv[2], argv[3], argv[4], col, perm, nz, jd, val, b, x, N, M, MAXNZ);

#ifdef EBUG
  printf("read data over\n");
  printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
#endif

  printf("Max = %d, Min = %d\n", nz[0], nz[N - 1]);
  int nz_sum = 0;
  for(int i = 0;i<N-1;i++){
    /* printf("%d %d\n", i, nz[i]); */
    nz_sum+=nz[i];
  }
  printf("Avg = %3.1f\n", (double)nz_sum/(double)N);

   free(b); 
   free(x); 
   free(val);
   free(col);
   free(perm); 
   free(nz);
   free(jd);

  return 0;
}

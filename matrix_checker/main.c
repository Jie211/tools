#include "header.h"

int main(int argc, char const* argv[])
{
  int N;
  int M;
  double *b;
  double *x;

  double *val;
  int *col;
  int *ptr;

  if(argc!=4)
  {
    printf("Usage error\n");
    printf("%s colval ptr bx \n", argv[0]);
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


  val=(double *)malloc(sizeof(double)*M);
  col=(int *)malloc(sizeof(int)*M);
  ptr=(int *)malloc(sizeof(int)*(N+1));
  if(!val || !col || !ptr)
  {
    printf("malloc val col ptr error");
    if(b)
      free(b);
    if(x)
      free(x);
    if(val)
      free(val);
    if(col)
      free(col);
    if(ptr)
      free(ptr);
    exit(1);
  }
#ifdef EBUG  
  printf("read data\n");
#endif
  getdata(argv[1],argv[2],argv[3],col,ptr,val,b,x,N,M);
#ifdef EBUG
  printf("read data over\n");
  printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
#endif

  double *diagvec;
  diagvec=(double *)malloc(sizeof(double)*N);
  double sum=0.0;
  int badSum1=0.0;
  int badSum2=0.0;
  for(int i=0;i<N;i++){
    sum=0.0;
    for(int j=ptr[i];j<ptr[i+1];j++){
      if(col[j] == i){
        diagvec[i]=val[j];
        if(diagvec[i] == 0){
          printf("!!diag zero!!");
          badSum1++;
        }
      }else{
        sum+=val[j];
      }
    }
    if(diagvec[i] < sum){
      badSum2++;
    }
  }
  printf("==================\n");
  if(badSum2 + badSum1 == 0){
    printf("== good\n");
  }else{
    printf("== bad\n");
    printf("---\ndiagZero = %d %.3f%%  notDominant = %d %.3f%% \n",badSum1,(double)badSum1/N, badSum2,(double)badSum2/N);
  }

  free(b);
  free(x);
  free(val);
  free(col);
  free(ptr);
  free(diagvec);
  return 0;
}

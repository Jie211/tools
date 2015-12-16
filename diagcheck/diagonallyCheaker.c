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
    printf("file error");
    exit(0);
  }
  printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
  printf("get headpart and check headerror\n");
  gethead(argv[1],argv[2],argv[3],&N,&M); 
  printf("get N = %d M = %d\n", N, M);
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
  printf("read data\n");
  getdata(argv[1],argv[2],argv[3],col,ptr,val,b,x,N,M);
  printf("read data over\n");
  printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

  /* for(int i=0;i<N;i++){ */
  /*   printf("%d %d %12e\n",col[i],ptr[i],val[i]); */
  /* } */
int count=0;
long int sumRow=0;
double diag=0.0;
int passFlag=1;
int falseCount=0;

  for(int i = 0;i<N;i++){
    sumRow=0;
    diag=0.0;
    for(int j = ptr[i];j<ptr[i+1];j++)
     {
       /* printf("%d %d %12e\n",count,col[j],val[j]); */
       if(count == col[j]){
        diag = fabs(val[j]);
       }else
       sumRow+=val[j];
     }
      /* printf("sum %ld,diag %f\n",sumRow, diag); */
    if(diag >= fabs(sumRow)){
      if(diag == fabs(sumRow)){
        printf("sum = diag\n");
      }
    }else{
      /* printf("in row %d ",count); */
      /* printf("sum=%ld,diag=%f\n",sumRow, diag); */
      falseCount++;
      passFlag=0;
    }
     count++;
   }
if(passFlag){
  printf("pass the test!\n");
}else{
  printf("%d/%d row false\n",falseCount, N);
}
  free(b);
  free(x);
  free(val);
  free(col);
  free(ptr);

  return 0;
}

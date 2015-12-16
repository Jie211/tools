#include<stdio.h>
int main(void)
{
  double epsmacd;
  float epsmacs;
  double d=1.0;
  float s=1.0;
  epsmacd=1.0;
  epsmacs=1.0;
  while(d+epsmacd>1.0)
  {
    epsmacd/=2.0;
  }
  while(s+epsmacs>1.0)
  {
    epsmacs/=2.0;
  }
  printf("double machine epsilon =%10.9e\n",epsmacd*2);
  printf("single machine epsilon =%10.9e\n",epsmacs*2);
}

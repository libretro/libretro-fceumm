#include <stdio.h>

#define MAX        512

void main(void)
{
 char buf[256];
 int count=0;

 while(fgets(buf,256,stdin)>0)
 {
  double p;

  if(sscanf(buf,"%lf",&p)==1)
  {
   p*=65536*16;
   printf("%ld,\n",(long)p);
   count++;
   if(count==MAX) break;
  }

 }

}

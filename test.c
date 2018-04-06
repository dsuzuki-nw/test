#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.141592653589793

int main(int argc, char** argv)
{
  int w,h;
  int cx,cy,r;
  unsigned char *in,*out;
  char buf[255];
  FILE *fp;
  int cp[1000],cv[1000],cpx[1000],cpy[1000];

  if(argc!=5) {
    printf("usage: %s input.pgm cx cy radius\n",argv[0]);
    exit(1);
  }

  fp = fopen(argv[1],"rb");
  fgets(buf,255,fp);
  fgets(buf,255,fp);
  fscanf(fp,"%d %d",&w,&h);
  fgets(buf,255,fp);
  fgets(buf,255,fp);

  cx = atoi(argv[2]);
  cy = atoi(argv[3]);
  r  = atoi(argv[4]);
  printf("w=%d th=%d center=%d,%d radius=%d\n",w,h,cx,cy,r);

  in = (unsigned char *)malloc(sizeof(unsigned char)*w*h);
  out = (unsigned char *)malloc(sizeof(unsigned char)*w*h*3);
  fread(in,sizeof(unsigned char),w*h,fp);
  fclose(fp);

  for(int i=0; i<w*h; i++) {
    out[i*3  ] = in[i];
    out[i*3+1] = in[i];
    out[i*3+2] = in[i];
  }

  int prex=0,prey=0,n=0,max=0,min=255;
  for(float i=0; i<360; i+=0.5) {
    int x = (int)(cx + r * cos(i * PI / 180.0) + 0.5);
    int y = (int)(cy + r * sin(i * PI / 180.0) + 0.5);
    if(x==prex && y==prey) continue;
    printf("%f\t%d\t%d\t%d\n",i,x,y,in[y*w+x]);
    out[(y*w+x)*3] = 255;
    cp[n] = n;
    cv[n] = in[y*w+x];
    if(cv[n]>max) max=cv[n];
    if(cv[n]<min) min=cv[n];
    cpx[n] = x;
    cpy[n] = y;
    prex = x;
    prey = y;
    n++;
  }

  int thr = (int)(min + (max-min)/3.0 + 0.5);
  printf("n=%d min=%d max=%d thr=%d\n",n,min,max,thr);
  //thr = 0;

  int m=0;
  for(int i=1; i<n-1; i++) {
    if( ((cv[i]>cv[i-1] && cv[i]>cv[i+1]) || cv[i]==cv[i-1]) && (cv[i]>thr) ) {
      out[(cpy[i]*w + cpx[i])*3  ] = 0;
      out[(cpy[i]*w + cpx[i])*3+1] = 255;
      out[(cpy[i]*w + cpx[i])*3+2] = 0;
      m++;
    }
  }

  if(cv[0]>cv[n-1] && cv[0]>cv[1] && cv[0]>thr) {
    out[(cpy[0]*w + cpx[0])*3  ] = 0;
    out[(cpy[0]*w + cpx[0])*3+1] = 255;
    out[(cpy[0]*w + cpx[0])*3+2] = 0;
    m++;
  }
  if(cv[n-1]>cv[n-2] && cv[n-1]>cv[0] && cv[n-1]>thr) {
    out[(cpy[n-1]*w + cpx[n-1])*3  ] = 0;
    out[(cpy[n-1]*w + cpx[n-1])*3+1] = 255;
    out[(cpy[n-1]*w + cpx[n-1])*3+2] = 0;
    m++;
  }

  printf("%d\n",m);
  fp = fopen("out.pgm","wb");
  fprintf(fp,"P6\n%d %d\n%d\n",w,h,255);
  for(int i=0; i<w*h; i++) {
    fprintf(fp,"%c%c%c",out[i*3],out[i*3+1],out[i*3+2]);
  }
  fclose(fp);

  free(in);
  free(out);
}

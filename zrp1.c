#include <stdio.h>
#include <time.h>

#include "k2.c"
#include "r1.c"
#include "serv.c"

#define LOGNAME "zrp.log"

unsigned char rnd[32],rnd_1[32];

#include "k3.c"

int a3(float *x, float *y, float *abc)
{
 float m,r,n,z,t,v;
 if(x[1]==0.) return(-1);
 m=x[2]-(x[2]*x[2])/x[1];
 r=y[2]-y[1]*(x[2]*x[2])/(x[1]*x[1]);
 n=1-(x[2]*x[2])/(x[1]*x[1]);
 z=x[3]-(x[3]*x[3])/x[1];
 t=1-(x[3]*x[3])/(x[1]*x[1]);
 v=y[3]-y[1]*(x[3]*x[3])/(x[1]*x[1]);
 if(m==0.) return(-2);
 if((m*t-z*n)==0.) return(-3);
 abc[2]=(m*v-r*z)/(m*t-z*n);
 abc[1]=(r-n*abc[2])/m;
 abc[0]=(y[1]-x[1]*abc[1]-abc[2])/(x[1]*x[1]);
 return(0);
 }

float p3(float x, float *abc)
{
 float r;
 r=abc[0]*x*x+abc[1]*x+abc[2];
 return(r);
}

#define N_PRM 210
//-----
// Ошибки -1 - превышен размер подстановки
// -2 - превышено число попыток
// -3 - ошибка при генерации (не совпадает сумма элементов)
int GenPermit(int n, unsigned char *prm)
    {
     int i,j,k,sum1,sum2,tg,c;
     unsigned char p;
     if(n>=N_PRM) return(-1);
     sum1=0;
     for(i=0;i<n;i++) sum1=sum1+i;
     NextRandom16(rnd,rnd_1,rnd);
     prm[0]=rnd[0]%n;
     c=0;
     for(i=1;i<n;i++)
        {
         m:;
         if(c>N_PRM*20) return(-2);
         NextRandom16(rnd,rnd_1,rnd);
         p=rnd[0]%n;
         tg=0;
         for(j=0;j<i;j++) if(p==prm[j]) tg=1;
         if(tg==0) prm[i]=p;
         else goto m;
        }
     sum2=0;
     for(i=0;i<n;i++) sum2=sum2+prm[i];
 //    printf("%03d %03d\n",sum1,sum2);
     if(sum1!=sum2) return(-3);
     for(i=0;i<n;i++) prm[i]=prm[i]+1;
    
     return(0);
    }     

int GenPermit_1( int n, unsigned char *prm, unsigned char *prm_1)
    {
     int i;
     if(n>=N_PRM) return(-1);
     for(i=0;i<n;i++)prm_1[prm[i]-1]=i+1;
     return(0);
    }

int MulPermit( int n, unsigned char *prm1, unsigned char *prm2, unsigned char *prm_r)
    {
     int i;
     if(n>=N_PRM) return(-1);
     for(i=0;i<n;i++) prm_r[i]=prm2[prm1[i]-1];
     return(0);
    }
// Процедура имитация посадки
int boarding(int n, unsigned char *qu,int echo)
     {
      unsigned char mm[N_PRM], mm1[N_PRM], pl[N_PRM],p;
      int i,j,k,c,tg;
      for(i=0;i<n;i++) {mm[i]=0;mm1[i]=1;}
//   список мест - это обратная подстановка
      GenPermit_1(n,qu,pl);
      NextRandom16(rnd,rnd_1,rnd);
      p=rnd[0]%n;     
//   Первый задумчивый пассажир
      mm[0]=p+1;
      mm1[p]=0;
      if(echo==1)
        {
         printf("Pass %03d Tic %03d Place %03d\n",1,qu[0],p+1);
         getch();
        }
     if(echo==2) AppLogD1(LOGNAME,n,mm);
//   Посадка остальных пассажиров, начиная со второго
      for(i=1;i<n;i++)
         {
          tg=0;
          for(j=0;j<i;j++)
             {
              if(mm[j]!=qu[i]) continue;
              else {tg=1;break;}
             }
//        Если место свободно
           if(tg==0)
             {
              mm[i]=qu[i];
              mm1[mm[i]-1]=0;
              if(echo==1){
              printf("Pass %03d Tic %03d Place %03d - Normal\n",i+1,qu[i],mm[i]);
              for(k=0;k<n;k++) printf("%03d ",mm[k]);
              printf("\n");
              getch();}
             }     
//        Если место занято
         else
            {
             for(j=0;j<n;j++) if(mm1[j]==1) {mm[i]=j+1;mm1[j]=0;break;}
              if(echo==1){
              printf("Pass %03d Tic %03d Place %03d - Wrong\n",i+1,qu[i],mm[i]);
              for(k=0;k<n;k++) printf("%03d ",mm[k]);
              printf("\n");
              getch();}
            }
          if(echo==2) AppLogD1(LOGNAME,n,mm);
         }
// Контроль рассадки
      c=0;
      for(i=0;i<n;i++) c=c+(mm[i]-1);
      tg=0;
      for(i=0;i<n;i++) tg=tg+i;

// Сколько не на своих местах
      c=0;tg=0;
      for(i=0;i<n;i++) if(qu[i]!=mm[i]) c++;

      if(qu[n-1]!=mm[n-1]) tg=1;

      if(tg==1) return(-c);
      else      return(c);
     }

int main(int argc, char *argv[])
      {
       int rez,i,j,sum1,sum2;
       float med1,med2;
       int td[6],td1[6];
       unsigned char mm[160],kk[160],kk1[160],ot[16],yyy[64],mkm[32];
       unsigned char km[32],m[32],userx[32];

       int pr_n=20;
       unsigned char prm1[N_PRM],prm2[N_PRM],prm3[N_PRM];
       float xx[4],yy[4],ab[3],r,d,ss[250],pp[250];
                                          
//                                                              
//   Current Time and Date
        GetTimeDate(td);
        AppLogT(LOGNAME,"Start ZRP");
        #include "k_t.c"
        AppLogT(LOGNAME,"Success Protect Function");
        
        rnd[0]=td[0];
        rnd[4]=td[1];
        rnd[6]=td[2];
        rnd[1]=td[3];
        rnd[5]=td[4];
        rnd[11]=td[5]-2000;

        IRandom(rnd_1,td[0]*td[1]);
        IRandom(rnd,td[2]+td[3]);
        rez=0;
        for(i=0;i<16;i++) if(rnd[i]!=rnd_1[i]) rez++;
        if(rez==0)
          {
           printf("Error Random (0)");
           AppLogT(LOGNAME,"Error Random (0)");
           return(-4);
          }
        for(i=0;i<16;i++) rnd[i]=rnd[i]^rnd_1[i];

        rez=InitRandom(rnd);
        if(rez==-1)
         {
          Crandom(td[0]*td[1]+td[2]);
         }
        rez=InitRandom(rnd);
        if(rez!=0)
         {
          printf("Error Random (1)");
          AppLogT(LOGNAME,"Error Random (1)");
          return(-4);
         }
        rez=TestVector(rnd,0);
        if(rez!=0)
          {
           printf("Error Random (2)");
           AppLogT(LOGNAME,"Error Random (2)");
           return(-4);
          }
        NextRandom16(rnd,rnd_1,rnd);
        rez=TestVector(rnd,0);
        if(rez!=0)
          {
           printf("Error Random (3)");
           AppLogT(LOGNAME,"Error Random (3)");
           return(-4);
          }
        printf("Ok Test Random\n");
        AppLogT(LOGNAME,"Ok Test Random");
        rez=UpRandom(td[0]*td[1]+td[2]+td[3]);
        if(rez!=0)
         {
          printf("Error Update Random File\n");
          AppLogT(LOGNAME,"Error Update Random File");
          return(-5);
         } 
//        for(i=0;i<10;i++)
//           printf("%03d ",rnd[i]);
//        printf("\n");
//     Генерация подстановки
        printf("N=%03d\n",pr_n);
        rez=GenPermit(pr_n,prm1);
        if(rez!=0)
          {
           printf("Error permit generation\n");
           AppLogT(LOGNAME,"Error permit generation");
          }
//       for(i=0;i<pr_n;i++)
//           printf("%03d ",prm1[i]); 
//        printf("\n");
//     Еще одна генерация
        rez=GenPermit(pr_n,prm1);
        if(rez!=0)
          {
           printf("Error permit generation\n");
           AppLogT(LOGNAME,"Error permit generation");
          }
//   Печать номеров очереди
       for(i=0;i<pr_n;i++)
           printf("%03d ",i+1); 
        printf("\n");
//   Печать очереди пассажиров
        for(i=0;i<pr_n;i++)
           printf("%03d ",prm1[i]); 
        printf("\n");
//     Вычисление обратной подстановки
        rez=GenPermit_1(pr_n,prm1,prm2);
        if(rez!=0)
          {
           printf("Error permit generation\n");
           AppLogT(LOGNAME,"Error permit generation");
          }
       for(i=0;i<pr_n;i++)
           printf("%03d ",prm2[i]); 
        printf("\n");
//    Умножение подстановок
        rez=MulPermit(pr_n,prm1,prm2,prm3);
        if(rez!=0)
          {
           printf("Error permit generation\n");
           AppLogT(LOGNAME,"Error permit generation");
          }
//       for(i=0;i<pr_n;i++)
//          printf("%03d ",prm3[i]); 
//       printf("\n");
        AppLogD1(LOGNAME,pr_n,prm3);
        AppLogD1(LOGNAME,pr_n,prm1);
        AppLogD1(LOGNAME,pr_n,prm2);
        rez=boarding(pr_n,prm1,2);
        for(i=0;i<250;i++) {ss[i]=0.;pp[i]=0.;};
        for(i=1;i<13;i++)
           {
            pr_n=i*10;
            sum1=0;sum2=0;
            for(j=0;j<100;j++)
               {
                GenPermit(pr_n,prm1);
                rez=boarding(pr_n,prm1,0);
                sum1=sum1+abs(rez);
               if(rez<0) sum2++;
               }
            printf("\nN= %03d Med1 =%f Med2 = %f\n",pr_n,(float)(sum1/100.),(float)(sum2/100.)); 
            AppLogD2(LOGNAME,pr_n,sum1,sum2);
            ss[pr_n]=(float)(sum1/100.);
            pp[pr_n]=(float)(sum2/100.);
           }   
           pr_n=200;   
           for(j=0;j<100;j++)
               {
               
                GenPermit(pr_n,prm1);
                rez=boarding(pr_n,prm1,0);
                sum1=sum1+abs(rez);
               if(rez<0) sum2++;
               }
            printf("\nN= %03d Med1 =%f Med2 = %f\n",pr_n,(float)(sum1/100.),(float)(sum2/100.)); 
            AppLogD2(LOGNAME,pr_n,sum1,sum2);
            ss[200]=(float)(sum1/100.);pp[200]=(float)(sum2/100.);
        xx[1]=20.;
        xx[2]=100.;
        xx[3]=200.;
        yy[1]=ss[20];
        yy[2]=ss[100];
        yy[3]=(float)(sum1/100.);
        r=a3(xx,yy,ab);
        AppLogD3(LOGNAME,ab[0],ab[1],ab[2]);
        for(i=1;i<21;i++)
           AppLogD4(LOGNAME,ss[i*10],p3((float)(i*10),ab),ss[i*10]-p3((float)(i*10),ab));
        xx[1]=20.;
        xx[2]=100.;
        xx[3]=200.;
        yy[1]=pp[20];
        yy[2]=pp[100];
        yy[3]=(float)(sum2/100.);
        r=a3(xx,yy,ab);
        AppLogD3(LOGNAME,ab[0],ab[1],ab[2]);
        for(i=1;i<21;i++)
           AppLogD4(LOGNAME,pp[i*10],p3((float)(i*10),ab),pp[i*10]-p3((float)(i*10),ab));
              
        printf("Successful ZRP ended!\n");
        AppLogT(LOGNAME,"Successful ZRP ended!");
        getch();
        return(0);
       }
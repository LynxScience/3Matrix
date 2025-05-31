//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <time.h>

#include "k2.c"
#include "r1.c"
#include "serv.c"

#define LOGNAME "mat.log"

unsigned char rnd[32],rnd_1[32];

#include "k3.c"
double det;
// Вычисление значения полинома
double poli( double x1, double x2)
{
 double y=x1+x2-3*x1*x2+(x1*x1)*x2+x1*(x2*x2)-(x1*x1)*(x2*x2);
 return y;
}
int gist[10];
void gg(float a)
     {
      if(a<0.) a=-a;
      if((a>0  )&&(a<=0.1)) gist[0]++;
      if((a>0.1)&&(a<=0.2)) gist[1]++;
      if((a>0.2)&&(a<=0.3)) gist[2]++;
      if((a>0.3)&&(a<=0.4)) gist[3]++;
      if((a>0.4)&&(a<=0.5)) gist[4]++;
      if((a>0.5)&&(a<=0.6)) gist[5]++;
      if((a>0.6)&&(a<=0.7)) gist[6]++;
      if((a>0.7)&&(a<=0.8)) gist[7]++;
      if((a>0.8)&&(a<=0.9)) gist[8]++;
      if((a>0.9)&&(a< 1.0)) gist[9]++;
     }
  
// Создание тестовой матрицы
double** TInput1() 
{
  double** p;
  int n=3;
  p = (double**)malloc(n * sizeof(double*));
  for (int i = 0; i < n; i++) 
      {
       p[i] = (double*)malloc(n * sizeof(double));
       for (int j = 0; j < n; j++) p[i][j]=(i+j)%2;
      }
  p[0][0]=0.25; 

  return p;
}

// Создание тестовой матрицы
double** TInput2(int n) 
{
  double** p;
  
  p = (double**)malloc(n * sizeof(double*));
  for (int i = 0; i < n; i++) 
      {
       p[i] = (double*)malloc(n * sizeof(double));
       NextRandom16(rnd,rnd_1,rnd);
       for (int j = 0; j < n; j++) p[i][j]=rnd[j]%2;
      }
  NextRandom16(rnd,rnd_1,rnd);
  p[rnd[0]%n][rnd[1]%n]=0.25; 

  return p;
}
// Создание тестовой матрицы в поле GF(2)
double** TInputMod2(int n) 
{
  double** p;
  
  p = (double**)malloc(n * sizeof(double*));
  for (int i = 0; i < n; i++) 
      {
       p[i] = (double*)malloc(n * sizeof(double));
       NextRandom16(rnd,rnd_1,rnd);
       for (int j = 0; j < n; j++) p[i][j]=rnd[j]%2;
      }
  NextRandom16(rnd,rnd_1,rnd);
  
  return p;
}


// Вывод на экран элементов матрицы
void Output(double** mas, int rows, int cols) 
{
  for (int i = 0; i < rows; i++)
      {
       for (int j = 0; j < cols; j++)
          printf("%8.4lf ", mas[i][j]);
       printf("\n");
      }
}

// Транспонирование матрицы
double** Transpone(double** mas, int rows, int cols)
{
  double** rez;
  rez = (double**)malloc(cols * sizeof(double*));
  for (int i = 0; i < cols; i++)
  {
    rez[i] = (double*)malloc(rows * sizeof(double));
    for (int j = 0; j < rows; j++)
      rez[i][j] = mas[j][i];
  }
  return rez;
}

// Функция освобождения памяти, выделенной под матрицу
void Free(double** mas, int rows)
{
  if (mas == 0) return;    // если память не была выделена, выходим
  for (int i = 0; i < rows; i++)
    free(mas[i]);
  free(mas);
}

// Получение матрицы без i-й строки и j-го столбца
// (функция нужна для вычисления определителя и миноров)
double** GetMatr(double** mas, int rows, int cols, int row, int col) {
  int di, dj;
  double** p = (double**)malloc((rows - 1) * sizeof(double*));
  di = 0;
  for (int i = 0; i < rows - 1; i++) { // проверка индекса строки
    if (i == row)  // строка совпала с вычеркиваемой
      di = 1;    // - дальше индексы на 1 больше
    dj = 0;
    p[i] = (double*)malloc((cols - 1) * sizeof(double));
    for (int j = 0; j < cols - 1; j++) { // проверка индекса столбца
      if (j == col)  // столбец совпал с вычеркиваемым
        dj = 1;    // - дальше индексы на 1 больше
      p[i][j] = mas[i + di][j + dj];
    }
  }
  return p;
}

// Рекурсивное вычисление определителя
double Determinant(double** mas, int m) {
  int k;
  double** p = 0;
  double d = 0;
  k = 1; //(-1) в степени i
  if (m < 1) { printf("Определитель вычислить невозможно!"); return 0; }
  if (m == 1) { d = mas[0][0]; return(d); }
  if (m == 2) { d = mas[0][0] * mas[1][1] - (mas[1][0] * mas[0][1]); return(d); }
  if (m > 2) {
    for (int i = 0; i < m; i++) {
      p = GetMatr(mas, m, m, i, 0);
      d = d + k * mas[i][0] * Determinant(p, m - 1);
      k = -k;
    }
  }
  Free(p, m-1);
  return(d);
}

// Рекурсивное вычисление определителя
double Determinant2(double** mas, int m) {
  int k;
  double** p = 0;
  double d = 0;
  k = 1; //(-1) в степени i
  if (m < 1) { printf("Определитель вычислить невозможно!"); return 0; }
  if (m == 1) { d = mas[0][0]; return(d); }
  if (m == 2) { d = (int)(poli(mas[0][0],mas[1][1])+poli(mas[1][0],mas[0][1]))%2; return(d); }
  if (m > 2) {
    for (int i = 0; i < m; i++) {
      p = GetMatr(mas, m, m, i, 0);
      d = (int)(d + k * mas[i][0] * Determinant(p, m - 1))%2;
 //     d = poli(d,poli(k,poli(mas[i][0],Determinant2(p, m - 1))));
      k = -k;
    }
  }
  Free(p, m-1);
  return(d);
}

// Обратная матрица
double** Mreverse(double** mas, int m)
{
  double** rez = (double**)malloc(m * sizeof(double*));
  det = Determinant(mas, m); // находим определитель исходной матрицы
  for (int i = 0; i < m; i++)
  {
    rez[i] = (double*)malloc(m * sizeof(double));
    for (int j = 0; j < m; j++)
    {
      rez[i][j] = Determinant(GetMatr(mas, m, m, i, j), m - 1);
      if ((i + j) % 2 == 1)       // если сумма индексов строки и столбца нечетная
        rez[i][j] = -rez[i][j];    // меняем знак минора
//      rez[i][j] = rez[i][j] / det;
    }
  }
  return Transpone(rez, m, m);
}
// Обратная матрица
double** Mreverse2(double** mas, int m)
{
  double** rez = (double**)malloc(m * sizeof(double*));
  det = Determinant2(mas, m); // находим определитель исходной матрицы
  for (int i = 0; i < m; i++)
  {
    rez[i] = (double*)malloc(m * sizeof(double));
    for (int j = 0; j < m; j++)
    {
      rez[i][j] = Determinant2(GetMatr(mas, m, m, i, j), m - 1);
      if ((i + j) % 2 == 1)       // если сумма индексов строки и столбца нечетная
        rez[i][j] = -rez[i][j];    // меняем знак минора
//      rez[i][j] = rez[i][j] / det;
    }
  }
  return Transpone(rez, m, m);
}

// Умножение матриц
double** Mul( double** ma, double** mb, int m)
{
 int i,j,k;
 double** masr;
 masr = (double**)malloc(m *sizeof(double*));
 for(i = 0; i < m; i++)
    {
     masr[i] = (double*)malloc(m * sizeof(double));
     for(j = 0; j < m; j++)
        {
         masr[i][j] = 0;
         for(k = 0; k < m; k++)
         masr[i][j] += ma[i][k] * mb[k][j];
        }
     }
 return masr;
}
// Умножение матриц по mod2
double** Mul_2( double** ma, double** mb, int m)
{
 int i,j,k;
 double** masr;
 masr = (double**)malloc(m *sizeof(double*));
 for(i = 0; i < m; i++)
    {
     masr[i] = (double*)malloc(m * sizeof(double));
     for(j = 0; j < m; j++)
        {
         masr[i][j] = 0;
         for(k = 0; k < m; k++)
         masr[i][j] += ma[i][k] + mb[k][j];
         masr[i][j]=(int)masr[i][j]%2;
        }
     }
 return masr;
}

// Умножение матриц - в имитационном поле 
double** Mul2( double** ma, double** mb, int m)
{
 int i,j,k;
 double** masr;
 masr = (double**)malloc(m *sizeof(double*));
 for(i = 0; i < m; i++)
    {
     masr[i] = (double*)malloc(m * sizeof(double));
     for(j = 0; j < m; j++)
        {
         masr[i][j] = 0;
         for(k = 0; k < m; k++)
         masr[i][j] += poli(ma[i][k],mb[k][j]);
        }
     }
 return masr;
}

double** Mul3( double** ma, double** mb, int m)
{
 int i,j,k;
 double** masr;
 masr = (double**)malloc(m *sizeof(double*));
 for(i = 0; i < m; i++)
    {
     masr[i] = (double*)malloc(m * sizeof(double));
     for(j = 0; j < m; j++)
        {
         masr[i][j] = 0;
         for(k = 0; k < m; k++)
         masr[i][j] = poli( masr[i][j],poli(ma[i][k],mb[k][j]));
        }
     }
 return masr;
}

int main() 
{
  int n,cc;
  double** mas;
  double** mas_reverse;
  double** mas_e;
  int* b;
  int td[6],td1[6], rez, i,j,cdet,cdete,cu,cneu,coth,c0,c1,adet,au,aneu,ao,sum;
  unsigned char mm[160],kk[160],kk1[160],xx[16],ot[16],yy[16],yyy[64],mkm[32];
  char mes[256];

  system("chcp 1251");
  system("cls");
//   Current Time and Date
        GetTimeDate(td);
        AppLogT(LOGNAME,"Start Matrix");
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
        for(i=0;i<10;i++)
           printf("%03d ",rnd[i]);
        printf("\n");

  n=8;
  printf("размерность матрицы: %d\n",n);
  cdet=0;cdete=0;cu=0;cneu=0;c0=0;c1=0;
  adet=0;au=0;aneu=0;ao=0;
  for(i=0;i<10;i++) gist[i]=0;
// изучение свойств
  for(cc=0;cc<100;cc++)
     {
      mas = TInput2(n);
     
      if(cc%20==0){
                   printf("Исходная матрица: \n");
                   Output(mas, n, n);
                   AppLogT(LOGNAME,"Sourse 3L matrix");
                   AppLogM(LOGNAME,mas,n);
                  }
// Находим обратную матрицу
      mas_reverse = Mreverse(mas, n);
      if(det==0.) 
        {
         printf("DET = 0 Reverse Matrix not exist\n");
         AppLogT(LOGNAME,"DET = 0 Reverse Matrix not exist");
         cdet++;
         goto mm;}
 
         printf("DET = %8.4lf ", det);
         cdete++;cu=0;cneu=0;
         for(i=0;i<n;i++)
            for(j=0;j<n;j++)
               {
                if(mas_reverse[i][j]==  .25) cu++;
                if(mas_reverse[i][j]==-0.25) cu++;
                if(mas_reverse[i][j]==  .75) cneu++;
                if(mas_reverse[i][j]==-0.75) cneu++;

                if(mas_reverse[i][j]== 0.  ) c0++;
                if(mas_reverse[i][j]== 1.  ) c1++;
                if(mas_reverse[i][j]==-1.  ) c1++;
                gg(mas_reverse[i][j]);
               }

      au=au+cu;
      aneu=aneu+cneu;
      ao=ao+(n*n-cu-cneu);
      if(cc%20==0){
                   AppLogT(LOGNAME,"Reverse (normal) 3L matrix");
                   AppLogM(LOGNAME,mas_reverse,n);
                  }
//     getchar();
      mm:;
      Free(mas_reverse, n);
      Free(mas, n);
     }
    ao=ao-c0-c1;
    printf("\nAll det = %d [%f] U = %f ~U = %f Other = %f\n",cdete, cdete/100.,  (float)au/cdete, (float)aneu/cdete, (float)ao/cdete);
    sprintf(mes,"\nAll det = %d [%f] U = %f ~U = %f Other = %f\n",cdete, cdete/100.,  (float)au/cdete, (float)aneu/cdete, (float)ao/cdete);
    AppLogT(LOGNAME,mes);

    for(i=0;i<10;i++) printf("[%02d] ",i+1);
    printf("\n");
    for(i=0;i<10;i++) printf("%04d ", gist[i]);
    for(i=0;i<10;i++) sprintf(mes+5*i,"%04d ",gist[i]);
    AppLogT(LOGNAME,mes);

    printf("\n");
    sum=0;
    for(i=0;i<10;i++) sum=sum+gist[i];
    printf("0 = %d 1 = %d SUM = %d [%d][%d]\n", c0, c1, sum, c0+c1+sum+ao, n*n*cdete); 
 
// Теперь по mod2
  mas = TInputMod2(n);
  printf("Первая матрица: \n");
  Output(mas, n, n);
  AppLogT(LOGNAME,"First GF2 matrix");
  AppLogM(LOGNAME,mas,n);
  
  mas_reverse = TInputMod2(n);
  printf("\n Вторая матрица: \n");
  Output(mas_reverse, n, n);
  AppLogT(LOGNAME,"Second GF2 matrix");
  AppLogM(LOGNAME,mas_reverse,n);
  getchar();
  
  // Проверяем умножением
  mas_e = Mul3(mas, mas_reverse,n);
  Output(mas_e, n, n);
  AppLogT(LOGNAME,"Polinom mult");
  AppLogM(LOGNAME,mas_e,n);
  printf("\n");
  // Проверяем умножением
  mas_e = Mul_2(mas, mas_reverse,n);
  AppLogT(LOGNAME,"Classic mult");
  Output(mas_e, n, n);
  AppLogM(LOGNAME,mas_e,n);
  printf("\n");
  getchar();
  
  cu=0;cneu=0;c0=0;c1=0;
  adet=0;au=0;aneu=0;ao=0;
  for(i=0;i<10;i++) gist[i]=0;
// изучение свойств
  for(cc=0;cc<100;cc++)
     {
      mas = TInput2(n);
      mas_reverse = TInput2(n);
      mas_e = Mul3(mas, mas_reverse,n);
      cu=0;cneu=0;
      for(i=0;i<n;i++)
         for(j=0;j<n;j++)
            {
             if(mas_e[i][j]==  .25) cu++;
             if(mas_e[i][j]==-0.25) cu++;
             if(mas_e[i][j]==  .75) cneu++;
             if(mas_e[i][j]==-0.75) cneu++;

             if(mas_e[i][j]== 0.  ) c0++;
             if(mas_e[i][j]== 1.  ) c1++;
             if(mas_e[i][j]==-1.  ) c1++;
             gg(mas_e[i][j]);
            }
      au=au+cu;
      aneu=aneu+cneu;
      ao=ao+(n*n-cu-cneu);
  
      Free(mas_reverse, n);
      Free(mas, n);
      Free(mas_e,n);
     } 
     ao=ao-c0-c1;
     printf("\nU = %f ~U = %f Other = %f\n", (float)au/100., (float)aneu/100., (float)ao/100.);
     sprintf(mes,"\nU = %f ~U = %f Other = %f\n", (float)au/100., (float)aneu/100., (float)ao/100.);
     AppLogT(LOGNAME,mes);
     for(i=0;i<10;i++) printf("[%02d] ",i+1);
     printf("\n");
     for(i=0;i<10;i++) printf("%04d ",gist[i]);
     for(i=0;i<10;i++) sprintf(mes+5*i,"%04d ",gist[i]);
     AppLogT(LOGNAME,mes);

     printf("\n 0 = %d 1 = %d U = %d ~U = %d\n",c0,c1,cu,cneu);
     sum=0;
     for(i=0;i<10;i++) sum=sum+gist[i];
     printf("0 = %d 1 = %d SUM = %d [%d][%d]\n", c0, c1, sum, c0+c1+sum, n*n*100); 
  
  getchar();

  return 0;
}

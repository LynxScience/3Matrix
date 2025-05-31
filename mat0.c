#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
// ������� ����� ��������� �������
double** Input(int rows, int cols) 
{
  double** p;
  p = (double**)malloc(rows * sizeof(double*));
  for (int i = 0; i < rows; i++) 
  {
    p[i] = (double*)malloc(cols * sizeof(double));
    for (int j = 0; j < cols; j++)
    {
      printf("mas[%d][%d]= ", i, j);
      scanf("%lf", &p[i][j]);
    }
  }
  return p;
}

// �������� �������� �������
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
//  p[1][1]=1;
  return p;
}


// ����� �� ����� ��������� �������
void Output(double** mas, int rows, int cols) 
{
  for (int i = 0; i < rows; i++)
      {
       for (int j = 0; j < cols; j++)
          printf("%8.4lf ", mas[i][j]);
       printf("\n");
      }
}

// ���������������� �������
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

// ������� ������������ ������, ���������� ��� �������
void Free(double** mas, int rows)
{
  if (mas == 0) return;    // ���� ������ �� ���� ��������, �������
  for (int i = 0; i < rows; i++)
    free(mas[i]);
  free(mas);
}

// ��������� ������� ��� i-� ������ � j-�� �������
// (������� ����� ��� ���������� ������������ � �������)
double** GetMatr(double** mas, int rows, int cols, int row, int col) {
  int di, dj;
  double** p = (double**)malloc((rows - 1) * sizeof(double*));
  di = 0;
  for (int i = 0; i < rows - 1; i++) { // �������� ������� ������
    if (i == row)  // ������ ������� � �������������
      di = 1;    // - ������ ������� �� 1 ������
    dj = 0;
    p[i] = (double*)malloc((cols - 1) * sizeof(double));
    for (int j = 0; j < cols - 1; j++) { // �������� ������� �������
      if (j == col)  // ������� ������ � �������������
        dj = 1;    // - ������ ������� �� 1 ������
      p[i][j] = mas[i + di][j + dj];
    }
  }
  return p;
}

// ����������� ���������� ������������
double Determinant(double** mas, int m) {
  int k;
  double** p = 0;
  double d = 0;
  k = 1; //(-1) � ������� i
  if (m < 1) { printf("������������ ��������� ����������!"); return 0; }
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

// �������� �������
double** Mreverse(double** mas, int m)
{
  double** rez = (double**)malloc(m * sizeof(double*));
  double det = Determinant(mas, m); // ������� ������������ �������� �������
  for (int i = 0; i < m; i++)
  {
    rez[i] = (double*)malloc(m * sizeof(double));
    for (int j = 0; j < m; j++)
    {
      rez[i][j] = Determinant(GetMatr(mas, m, m, i, j), m - 1);
      if ((i + j) % 2 == 1)       // ���� ����� �������� ������ � ������� ��������
        rez[i][j] = -rez[i][j];    // ������ ���� ������
      rez[i][j] = rez[i][j] / det;
    }
  }
  return Transpone(rez, m, m);
}

// ��������� ������
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

int main() 
{
  int n;
  double** mas;
  int* b;
  system("chcp 1251");
  system("cls");
  n=3;
  printf("����������� �������: %d",n);
//  scanf("%d", &n);
//  mas = Input(n, n);
  mas = TInput1();
  printf("�������� �������: \n");
  Output(mas, n, n);
  // ������� �������� �������
  double** mas_reverse = Mreverse(mas, n);
  printf("\n�������� �������: \n");
  Output(mas_reverse, n, n);
  getchar();
  // ��������� ����������
  double** mas_e = Mul (mas, mas_reverse,n);
  Output(mas_e, n, n);
  // ����������� ������
  Free(mas_reverse, n);
  Free(mas, n);
  Free(mas_e,n);
  getchar();
  return 0;
}

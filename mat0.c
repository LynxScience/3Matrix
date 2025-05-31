#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
// Функция ввода элементов матрицы
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
//  p[1][1]=1;
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

// Обратная матрица
double** Mreverse(double** mas, int m)
{
  double** rez = (double**)malloc(m * sizeof(double*));
  double det = Determinant(mas, m); // находим определитель исходной матрицы
  for (int i = 0; i < m; i++)
  {
    rez[i] = (double*)malloc(m * sizeof(double));
    for (int j = 0; j < m; j++)
    {
      rez[i][j] = Determinant(GetMatr(mas, m, m, i, j), m - 1);
      if ((i + j) % 2 == 1)       // если сумма индексов строки и столбца нечетная
        rez[i][j] = -rez[i][j];    // меняем знак минора
      rez[i][j] = rez[i][j] / det;
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

int main() 
{
  int n;
  double** mas;
  int* b;
  system("chcp 1251");
  system("cls");
  n=3;
  printf("размерность матрицы: %d",n);
//  scanf("%d", &n);
//  mas = Input(n, n);
  mas = TInput1();
  printf("Исходная матрица: \n");
  Output(mas, n, n);
  // Находим обратную матрицу
  double** mas_reverse = Mreverse(mas, n);
  printf("\nОбратная матрица: \n");
  Output(mas_reverse, n, n);
  getchar();
  // Проверяем умножением
  double** mas_e = Mul (mas, mas_reverse,n);
  Output(mas_e, n, n);
  // Освобождаем память
  Free(mas_reverse, n);
  Free(mas, n);
  Free(mas_e,n);
  getchar();
  return 0;
}

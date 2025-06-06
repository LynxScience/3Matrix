# 3Matrix
The project contains the implementation of the multiplication of trinary matrices and the inversion of trinary matrices
Проект реализации тринарных функций содержит следующие файлы
The ternary functions implementation project contains the following files:
K_t.c – код на языке С, выполняющий процедуры тестирования используемых внутри проекта функций, включая функции получения случайных чисел и криптографические функции.
K_t.c –  C code that performs testing procedures for functions used within the project, including functions for obtaining random numbers and cryptographic functions.
K2.с – код на языке С, реализующий криптографические функции, необходимые для реализации алгортма подучения случайных векторов и чисел.
K2.с – a C code that implements the cryptographic functions necessary to implement the algorithm for learning random vectors and numbers.
K3.c – дополнительные криптографические функции, необходимые для работы в блокчейн и быстрой реализации криптографических алгоритмов.
K3.c – additional cryptographic functions required for operation in the blockchain and fast implementation of cryptographic algorithms.
Mat.c – основной файл проекта, содержащий функции умножения матриц, в нем необходимо указать изучаемый размер матрицы. В коде генерируются случайные матрицы с использованием реализованных алгоритмов генерации случайных чисел, реализуются алгоритмы умножения и обращения матриц. Операции над матрицами выполняются заданное количество раз для применения методов Монте-Карло.
Mat.c – the main project file containing the matrix multiplication functions, it is necessary to specify the studied matrix size. The code generates random matrices using implemented random number generation algorithms, and implements matrix multiplication and inversion algorithms. Operations on matrices are performed a set number of times to apply Monte Carlo methods.
Mmat.bat – компиляции проекта в исполняемый файл. В этом файле необходимо указать реальный путь к компилятору на сетевом или локальном диске.
Mmat.bat – – compilation of the project into an executable file. In this file, you must specify the actual path to the compiler on a network or local disk.
R1 – процедура тестирования случайных векторов на вырожденные свойства.
R1 – a procedure for testing random vectors for degenerate properties.
T и T1 – таблицы выполнения операций умножения, необходимые для работы криптографических алгоритмов.
T and T1 are tables for performing multiplication operations necessary for the operation of cryptographic algorithms.
Zpr.c – решение задачи «рассеянного пассажира», связанное с генерацией случайных очередей рассадки и подстановок, с учетом небинарной логики.
Zpr.c – solving the problem of the "distracted passenger", related to the generation of random queues of seating and substitutions, taking into account non-binary logic

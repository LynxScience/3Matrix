@echo on
del mat.exe
path=C:\MinGW32\bin;C:\MinGW32\msys\1.0\bin
gcc mat.c>!mess
ren a.exe mat.exe
pause

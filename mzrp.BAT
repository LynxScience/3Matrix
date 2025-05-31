@echo on
del zrp.exe
path=C:\MinGW32\bin;C:\MinGW32\msys\1.0\bin
gcc zrp.c>!mess
ren a.exe zrp.exe
pause

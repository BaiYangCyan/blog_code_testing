@echo off
chcp 65001 >nul
rem ============================================================
rem  Default run: skips the crash experiment (experiment A)
rem  To reproduce the heap-corruption crash (exit code 0xC0000374):
rem    g++ -std=c++11 -O2 -DRUN_CRASH vector_sim.cpp -o vector_crash
rem    vector_crash
rem ============================================================
where g++ >nul 2>nul
if errorlevel 1 (
  echo [build] g++ not found. Open output.txt directly to see all results.
  pause
  exit /b 1
)
g++ -std=c++11 -O2 -Wall -Wextra vector_sim.cpp -o vector_sim.exe
if errorlevel 1 (
  echo [build] compile failed. Check the messages above.
  pause
  exit /b 1
)
vector_sim.exe > output.txt
type output.txt
echo.
echo [build] done. output.txt refreshed.
pause

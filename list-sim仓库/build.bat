@echo off
chcp 65001 >nul
where g++ >nul 2>nul
if errorlevel 1 (
  echo [build] g++ not found. Open output.txt directly to see all results.
  pause
  exit /b 1
)
g++ -std=c++11 -O2 -Wall -Wextra list_sim.cpp -o list_sim.exe
if errorlevel 1 (
  echo [build] compile failed. Check the messages above.
  pause
  exit /b 1
)
list_sim.exe > output.txt
type output.txt
echo.
echo [build] done. output.txt refreshed.
pause

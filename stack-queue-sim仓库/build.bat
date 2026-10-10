@echo off
chcp 65001 >nul
where g++ >nul 2>nul
if errorlevel 1 (
  echo [build] g++ not found. Open output.txt directly to see all results.
  pause
  exit /b 1
)
g++ -std=c++11 -O2 -Wall -Wextra stack_queue_sim.cpp -o stack_queue_sim.exe
if errorlevel 1 (
  echo [build] compile failed. Check the messages above.
  pause
  exit /b 1
)
stack_queue_sim.exe > output.txt
type output.txt
echo.
echo [build] done. output.txt refreshed.
pause

@echo off
chcp 65001 >nul
rem One-click build: compile + run + refresh output.txt (includes split demos)
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0build.ps1"
type "%~dp0output.txt"
pause

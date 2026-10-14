# 一键编译 + 运行 + 刷新 output.txt（含 split 分离编译演示）
# 调用方式：双击 build.bat（bat 会转到这里执行）
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $root

$g = (Get-Command g++ -ErrorAction SilentlyContinue).Source
if (-not $g) {
    Write-Host '[build] g++ not found. Open output.txt directly to see all results.'
    exit 1
}

$enc = [System.Text.UTF8Encoding]::new($false)

# 1) 主程序
& $g -std=c++11 -O2 -Wall -Wextra template_advanced_sim.cpp -o template_advanced_sim.exe
if ($LASTEXITCODE -ne 0) { Write-Host '[build] compile failed.'; exit 1 }
cmd /c "template_advanced_sim.exe > output.txt"

# 2) split：方案一（推荐）
& $g -std=c++11 -O2 -Wall -Wextra split\good\a.cpp split\good\main.cpp -o split\good\good.exe
cmd /c "split\good\good.exe > split\good\run_out.txt"

# 3) split：坏例（预期链接失败）
& $g -std=c++11 -c split\bad\a.cpp -o split\bad\a.o 2>$null
& $g -std=c++11 -c split\bad\main.cpp -o split\bad\main.o 2>$null
cmd /c "`"$g`" split\bad\a.o split\bad\main.o -o split\bad\bad.exe 2> split\bad\link_error.txt"

# 4) split：方案二（显式实例化）
& $g -std=c++11 -O2 -Wall -Wextra split\explicit\a.cpp split\explicit\main.cpp -o split\explicit\explicit.exe
cmd /c "split\explicit\explicit.exe > split\explicit\run_out.txt"

# 5) 组装 output.txt
$main  = [System.IO.File]::ReadAllText("$root\output.txt", $enc)
$good  = [System.IO.File]::ReadAllText("$root\split\good\run_out.txt", $enc)
$err   = [System.IO.File]::ReadAllText("$root\split\bad\link_error.txt", $enc)
$expl  = [System.IO.File]::ReadAllText("$root\split\explicit\run_out.txt", $enc)

$text = $main + "`r`n========== 附：模板分离编译演示（split 子目录） ==========`r`n" +
    "【方案一（推荐）】声明和定义都放进头文件（split/good）`r`n运行输出：`r`n" + $good + "`r`n" +
    "【坏例】声明在 .h、定义在 .cpp（split/bad）`r`n编译通过，链接报错（g++/ld 真实输出，MSVC 下等价于 LNK2019）：`r`n" +
    $err + "链接失败，未生成可执行文件。`r`n`r`n" +
    "【方案二】定义在 .cpp + 显式实例化（split/explicit，工程上不推荐）`r`n运行输出：`r`n" + $expl

[System.IO.File]::WriteAllText("$root\output.txt", $text, $enc)

# 6) 清理中间产物
Remove-Item "$root\template_advanced_sim.exe",
    "$root\split\good\good.exe", "$root\split\good\run_out.txt",
    "$root\split\bad\a.o", "$root\split\bad\main.o", "$root\split\bad\link_error.txt",
    "$root\split\explicit\explicit.exe", "$root\split\explicit\run_out.txt" -Force -ErrorAction SilentlyContinue

Write-Host '[build] done. output.txt refreshed.'

# vector-sim：C++ vector 一站式实验代码（单文件版）

配套文章：《C++入门篇（十四）：vector 模拟实现——三指针、扩容流程与 memcpy 陷阱》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/details/167221493

## 一分钟上手

1. **只想看结果**：直接打开 `output.txt`（含崩溃实验的复现记录），不用装编译器
2. **想自己跑**：双击 `build.bat`（需要 g++），一键编译 + 运行 + 刷新 output.txt
3. **想找某段代码**：打开 `vector_sim.cpp`，按下面导览表 Ctrl+F 搜函数名

## 代码导览

| 文章章节 | 内容 | 搜索函数名 |
| --- | --- | --- |
| 第一~六节 + 第七节实验 B | 手写 vector 全部测试（正确版逐个深拷贝，全程安全） | `namespace bit` / `test_sim()` |
| 第七节 实验 C | memcpy + int 对照组（表面正常，掩盖 UB） | `test_memcpy_int()` |
| 第七节 实验 A | memcpy + 自定义类型（**会崩溃**，需 -DRUN_CRASH） | `test_memcpy_crash()` |
| 第十一节 | 扩容曲线对比（2 倍 / 1.5 倍两种规则） | `test_growth()` |

## 手动编译（Linux / macOS / 有 g++ 的环境）

```bash
# 默认运行（跳过崩溃实验）
g++ -std=c++11 -O2 -Wall -Wextra vector_sim.cpp -o vector_sim && ./vector_sim

# 复现崩溃实验（离开作用域析构时堆损坏，Windows 退出码 0xC0000374）
g++ -std=c++11 -O2 -DRUN_CRASH vector_sim.cpp -o vector_crash && ./vector_crash
```

## 说明

- 教学简化版，用于理解三指针、扩容流程与深浅拷贝，不代表标准库实现
- 生产代码请直接使用 `std::vector`
- 实测环境：w64devkit g++ 15.2.0（`-std=c++11` / `-O2`）/ VS2022 x64
- `output.txt` 由 `build.bat`（-O2）真实运行生成；末尾附 `-DRUN_CRASH` 崩溃复现记录

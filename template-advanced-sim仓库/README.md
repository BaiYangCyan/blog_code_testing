# template-advanced-sim：C++ 模板进阶 一站式实验代码

配套文章：《C++入门篇（十八）：模板进阶——非类型参数、特化全家桶与分离编译》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/待发布

## 一分钟上手

1. **只想看结果**：直接打开 `output.txt`（含分离编译的链接错误原文），不用装编译器
2. **想自己跑**：双击 `build.bat`（需要 g++），一键编译 + 运行 + 刷新 output.txt
3. **想找某段代码**：打开 `template_advanced_sim.cpp`，按下面导览表 Ctrl+F 搜函数名

## 代码导览

| 文章章节 | 内容 | 位置 |
| --- | --- | --- |
| 第一节 | 非类型模板参数：array&lt;T, N&gt; | `test_array()` |
| 第三节 | 函数模板特化：LessFunc 与 LessFunc&lt;Date*&gt; | `test_func_spec()` |
| 第四节 | 类模板全特化 / 偏特化：Data 家族 | `test_class_spec()` |
| 第五节 | 类模板特化实战：给 vector&lt;Date*&gt; 排序 | `test_sort_ptr()` |
| 第六节 | 分离编译三套演示 | `split/` 子目录 |

## split 子目录说明

- `split/bad`：声明在 a.h、定义在 a.cpp → 编译通过、链接报错（错误原文录在 output.txt）
- `split/good`：声明和定义都放进头文件（推荐做法）→ 正常运行
- `split/explicit`：定义留在 .cpp + 显式实例化 → 能跑，但工程上不推荐

## 手动编译（Linux / macOS / 有 g++ 的环境）

```bash
g++ -std=c++11 -O2 -Wall -Wextra template_advanced_sim.cpp -o template_advanced_sim && ./template_advanced_sim
```

## 说明

- 实测环境：w64devkit g++ 15.2.0（`-std=c++11` / `-O2`）/ VS2022 x64
- MSVC 下分离编译的链接错误是 `LNK2019`，g++ 下是 `undefined reference`，同一根因
- `output.txt` 由 `build.bat` / `build.ps1` 真实运行生成

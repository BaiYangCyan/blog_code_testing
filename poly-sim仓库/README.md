# poly-sim：C++ 多态 一站式实验代码（单文件版）

配套文章：《C++进阶篇（二）：多态——虚函数、重写、虚函数表与三个经典坑》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/待发布

## 一分钟上手

1. **只想看结果**：直接打开 `output.txt`（值传递反例、虚析构对照、sizeof、默认参数坑全在），不用装编译器
2. **想自己跑**：双击 `build.bat`（需要 g++），一键编译 + 运行 + 刷新 output.txt
3. **想找某段代码**：打开 `poly_sim.cpp`，按下面导览表 Ctrl+F 搜函数名

## 代码导览

| 文章章节 | 内容 | 搜索函数名 |
| --- | --- | --- |
| 第一节 | 买票多态初体验（基类引用调用） | `test_poly_basic()` |
| 第二节 | 构成条件对照：引用版 vs 值传递版 | `test_ref_vs_value()` |
| 第三节 | override / final / 协变 | `test_override_covariant()` |
| 第四节 | 重载 / 重写 / 隐藏 三兄弟 | `test_three_concepts()` |
| 第五节 | 虚析构对照实验（泄漏 vs 正确释放） | `test_virtual_dtor()` |
| 第六节 | 抽象类与纯虚函数 | `test_abstract()` |
| 第七节 | 虚表探针：sizeof（含变体）/ vfptr / 构造中调虚函数 | `test_vtable_probe()` |
| 第七节附 | 多继承虚表（两张表实测）+ 虚表住在哪 | `test_multi_inherit_vtable()` / `test_vtable_addr()` |
| 第八节 | 面试坑：默认参数（B->1） | `test_interview_traps()` |

## 手动编译（Linux / macOS / 有 g++ 的环境）

```bash
g++ -std=c++11 -O2 -Wall -Wextra poly_sim.cpp -o poly_sim && ./poly_sim
```

## 说明

- 实测环境：w64devkit g++ 15.2.0（`-std=c++11` / `-O2`）/ VS2022 x64
- sizeof / vfptr 地址等数据与编译器和平台有关，本仓库为 x64 实测值
- `output.txt` 由 `build.bat` 真实运行生成

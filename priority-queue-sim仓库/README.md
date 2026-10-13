# priority-queue-sim：C++ priority_queue + 仿函数 一站式实验代码（单文件版）

配套文章：《C++入门篇（十七）：priority_queue 完整模拟实现——仿函数、区间建堆与指针特化》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/待发布

## 一分钟上手

1. **只想看结果**：直接打开 `output.txt`，全部实验输出都在里面（不用装编译器）
2. **想自己跑**：双击 `build.bat`（需要 g++），一键编译 + 运行 + 刷新 output.txt
3. **想找某段代码**：打开 `priority_queue_sim.cpp`，按下面导览表 Ctrl+F 搜函数名

## 代码导览

| 文章章节 | 内容 | 搜索函数名 |
| --- | --- | --- |
| 第二节 | 仿函数：调用本质（`f(1,2)` ↔ `operator()`）、Less/Greater、sort 降序、find_if 条件 | `test_functor()` |
| 第三节 | 完整模拟实现：区间构造 + 建堆 + 向上/向下调整 | `namespace bit` / `test_build_heap()` |
| 第四节 | 自定义类型（Date + LessDate）与指针陷阱（`Less<T*>` 偏特化） | `test_custom_type()` / `test_pointer_trap()` |
| 第五节 | Top-K：前 K 大 / 第 K 大 / 前 K 小 | `test_topk()` |
| 附 | deque 排序效率实测（直接 sort vs 拷到 vector） | `test_deque_sort()` |

## 手动编译（Linux / macOS / 有 g++ 的环境）

```bash
g++ -std=c++11 -O2 -Wall -Wextra priority_queue_sim.cpp -o priority_queue_sim && ./priority_queue_sim
```

> deque 效率实验（附节）的真实数字需要 `-O2`。

## 说明

- 教学简化版，用于理解堆的调整、仿函数与模板特化动机，不代表标准库实现
- 生产代码请直接使用 `std::priority_queue`
- 实测环境：w64devkit g++ 15.2.0（`-std=c++11` / `-O2`）/ VS2022 x64
- `output.txt` 由 `build.bat`（-O2）真实运行生成

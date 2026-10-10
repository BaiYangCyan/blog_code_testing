# stack-queue-sim：C++ stack / queue / priority_queue 一站式实验代码（单文件版）

配套文章：《C++入门篇（十六）：stack 和 queue——容器适配器、deque 底层与手写模拟实现》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/待发布

## 一分钟上手

1. **只想看结果**：直接打开 `output.txt`，全部实验输出都在里面（不用装编译器）
2. **想自己跑**：双击 `build.bat`（需要 g++），一键编译 + 运行 + 刷新 output.txt
3. **想找某段代码**：打开 `stack_queue_sim.cpp`，按下面导览表 Ctrl+F 搜函数名

## 代码导览

| 文章章节 | 内容 | 搜索函数名 |
| --- | --- | --- |
| 第二节 | stack 实战：最小栈（OJ，双栈 O(1) 取最小值） | `class MinStack` / `test_min_stack()` |
| 第四节 | 容器适配器：同一套外壳换底层（deque / vector / list） | `test_adapter_switch()` |
| 第五节 | 手写 stack / queue 适配器 + 全部测试 | `namespace bit` / `test_stack_all()` / `test_queue_all()` |
| 第六节 | priority_queue 手写堆（向上 / 向下调整 + 仿函数控大堆小堆） | `test_priority_queue()` |
| 第七节 | deque 特性验证（双端 O(1) + 随机访问） | `test_deque()` |

## 手动编译（Linux / macOS / 有 g++ 的环境）

```bash
g++ -std=c++11 -O2 -Wall -Wextra stack_queue_sim.cpp -o stack_queue_sim && ./stack_queue_sim
```

## 说明

- 教学简化版，用于理解容器适配器与堆调整，不代表标准库实现
- 生产代码请直接使用 `std::stack` / `std::queue` / `std::priority_queue`
- 实测环境：w64devkit g++ 15.2.0（`-std=c++11` / `-O2`）/ VS2022 x64
- `output.txt` 由 `build.bat`（-O2）真实运行生成

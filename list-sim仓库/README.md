# list-sim：C++ list 一站式实验代码（单文件版）

配套文章：《C++入门篇（十五）：list——带头双向循环链表：接口、迭代器失效与手写模拟实现》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/待发布

## 一分钟上手

1. **只想看结果**：直接打开 `output.txt`，全部实验输出都在里面（不用装编译器）
2. **想自己跑**：双击 `build.bat`（需要 g++），一键编译 + 运行 + 刷新 output.txt
3. **想找某段代码**：打开 `list_sim.cpp`，按下面导览表 Ctrl+F 搜函数名

## 代码导览

| 文章章节 | 内容 | 搜索函数名 |
| --- | --- | --- |
| 第三节 3.1 | emplace_back 计数实验 | `test_emplace_back()` |
| 第三节 3.2/3.3 | 独门接口（splice / merge / unique / remove / reverse / swap / find） | `test_misc_apis()` |
| 第五节 | 手写模拟实现（结点 / 迭代器 / 反向迭代器 / list 类）+ 全部测试 | `namespace bit` / `test_*()` |
| 第六节 | 效率实测（list.sort vs 拷贝到 vector） | `test_sort_speed()` |
| 第七节 | erase 错误对照（安全自检，不触发 UB） | `test_erase_bug()` / `test_erase_ok()` |

## 手动编译（Linux / macOS / 有 g++ 的环境）

```bash
g++ -std=c++11 -O2 -Wall -Wextra list_sim.cpp -o list_sim && ./list_sim
```

> 效率实验（第六节）的真实数字需要 `-O2`；不加优化结论会失真。

## 说明

- 教学简化版，用于理解带头双向循环链表与迭代器封装，不代表标准库实现
- 生产代码请直接使用 `std::list`
- 实测环境：w64devkit g++ 15.2.0（`-std=c++11` / `-O2`）/ VS2022 x64
- `output.txt` 由 `build.bat`（-O2）真实运行生成

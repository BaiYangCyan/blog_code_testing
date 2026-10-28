# rbtree-sim：C++ 红黑树一站式实验（单文件版）

配套文章：《C++进阶篇（五）：红黑树——四条规则、三种插入情况与旋转变色》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/待发布

## 一分钟上手

1. **只想看结果**：直接打开 `output.txt`（五个插入用例、两组序列、10 万 RB/AVL/BST 对比都在里面）
2. **想自己跑**：双击 `build.bat`（需要 g++），一键编译 + 运行 + 刷新 output.txt
3. **想找某段代码**：打开 `rbtree_sim.cpp`，按下面导览表 Ctrl+F 搜函数名

## 代码导览

| 文章章节 | 内容 | 搜索 |
| --- | --- | --- |
| 结点与插入 | `Colour`、`RBTreeNode`、`Insert`、`RotateL/R` | `namespace rb` |
| 五个插入用例 | 叔红变色 / LL / RR / LR / RL | `test_minimal_cases()` |
| 两组课堂序列 | 常规序列、双旋序列 | `test_course_sequences()` |
| 10 万有序 | RB vs AVL vs BST 高度、旋转、查找 | `test_ordered_100k()` |
| 10 万随机 | RB vs AVL + 重复 key | `test_random_100k()` |
| AVL 对照 | 只保留插入、查找、旋转计数 | `namespace avl_cmp` |

## 本次实测摘要（详见 output.txt）

```text
五个最小用例：
  叔叔为红：0 次旋转，2 次变色事件
  LL / RR：1 次单旋
  LR / RL：2 次单旋

两组课堂序列：
  常规序列：高度 4，单旋 8，变色事件 9
  双旋序列：高度 4，单旋 1，变色事件 5

10 万有序插入：
  RB ：高度 31，单旋 99969，变色事件 199948
  AVL：高度 17，单旋 99983
  BST：高度 100000（单链）
  2000 次查找：RB 0.550 ms / AVL 0.564 ms / BST 399.617 ms

10 万随机插入：
  RB ：高度 20，单旋 58391，变色事件 90348
  AVL：高度 20，单旋 69765
  再插一遍：100000 次全部失败，Size 仍为 100000
```

## 手动编译（Linux / macOS / 有 g++ 的环境）

```bash
g++ -std=c++11 -O2 -Wall -Wextra rbtree_sim.cpp -o rbtree_sim && ./rbtree_sim
```

## 说明

- 实测环境：w64devkit g++ 15.2.0（`-std=c++11` / `-O2`）/ VS2022 x64
- `output.txt` 由 `build.bat` 真实运行生成，时间数字会随机器波动
- 旋转次数按“单旋”统计，双旋记 2 次；变色次数按“事件”统计，一次可能改 2~3 个结点
- 普通 BST 的 10 万单链是直接构造的：结构与 1..N 有序插入完全一致，省掉 O(N²) 建树等待
- `IsBalanceTree()` 检查：根黑、无红红、黑高相等、parent 指针、BST 中序有序
- `DumpInline()` 输出 `key(R/B)[左, 右]`，插入过程一眼看清颜色

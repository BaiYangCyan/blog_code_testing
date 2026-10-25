# avl-sim：C++ AVL 树一站式实验（单文件版）

配套文章：《C++进阶篇（四）：AVL 树——从 BST 退化到自平衡，四种旋转全拆解》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/待发布

## 一分钟上手

1. **只想看结果**：直接打开 `output.txt`（四种旋转、双旋 BF 三案例、10 万结点对比全在里面）
2. **想自己跑**：双击 `build.bat`（需要 g++），一键编译 + 运行 + 刷新 output.txt
3. **想找某段代码**：打开 `avl_sim.cpp`，按下面导览表 Ctrl+F 搜函数名

## 代码导览

| 文章章节 | 内容 | 搜索函数名 |
| --- | --- | --- |
| 第一节 | AVL 结点 + 插入 + BF 更新 + 四种旋转 + 查找 + 自检 | `namespace avl` |
| 第二节 | 四种旋转最小用例（LL/RR/LR/RL） | `test_basic_rotations()` |
| 第三节 | 双旋 BF 三种修正（旧 bf = -1 / 0 / +1） | `test_double_bf_cases()` |
| 第四节 | 两组课堂序列 | `test_course_sequences()` |
| 第五节 | 10 万有序：AVL vs 普通 BST | `test_ordered_vs_bst()` |
| 第六节 | 10 万随机插入 + 四道自检 | `test_random_insert()` |

## 本次实测摘要（详见 output.txt）

```text
四种旋转：LL→右单旋，RR→左单旋，LR/RL→双旋，旋转后全部平衡
双旋 BF：subLR/subRL 旧 bf = -1 / 0 / +1 六种场景全部命中并平衡

10 万有序插入：
  AVL：高度 17，Size 100000，平衡
  BST：高度 100000（退化成单链）
  2000 次随机查找：AVL 0.214 ms，BST 216.996 ms（约 1014 倍）

10 万随机插入：
  高度 20，Size 100000，平衡
  再插一遍：100000 次全部失败（key 不重复），Size 仍为 100000
```

## 手动编译（Linux / macOS / 有 g++ 的环境）

```bash
g++ -std=c++11 -O2 -Wall -Wextra avl_sim.cpp -o avl_sim && ./avl_sim
```

## 说明

- 实测环境：w64devkit g++ 15.2.0（`-std=c++11` / `-O2`）/ VS2022 x64
- `output.txt` 由 `build.bat` 真实运行生成，时间数字会随机器波动
- 普通 BST 的 10 万单链是**直接构造**的：结构与 1..N 有序插入完全一致，但省掉了 O(N²) 的建树等待
- `IsBalanceTree()` 不只检查高度差，还检查 BF 一致性、parent 指针、BST 中序有序，共四道自检
- `DumpInline()` 输出 `key(bf)[左, 右]`，旋转前后对照非常直观

# bst-mapset-sim：C++ 二叉搜索树 + map/set 一站式实验代码（单文件版）

配套文章：《C++进阶篇（三）：二叉搜索树与 map/set——从手写 BST 到会用关联容器》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/待发布

## 一分钟上手

1. **只想看结果**：直接打开 `output.txt`（高度退化、查找耗时、删除纠偏、词频全在里面），不用装编译器
2. **想自己跑**：双击 `build.bat`（需要 g++），一键编译 + 运行 + 刷新 output.txt
3. **想找某段代码**：打开 `bst_mapset_sim.cpp`，按下面导览表 Ctrl+F 搜函数名

## 代码导览

| 文章章节 | 内容 | 搜索函数名 |
| --- | --- | --- |
| 第一节 | BST 基本操作（插入/查找/递归版/中序） | `test_bst_basic()` |
| 第二节 | 删除四种情况（叶子/单孩子/双孩子/根） | `test_bst_erase()` |
| 第三节 | 有序退化 vs 随机（高度 + 查找耗时实测） | `test_bst_performance()` |
| 第四节 | KV 模型（词频/词典/深拷贝）+ 删除纠偏 | `test_kv_bst()` |
| 第五节 | set（去重排序/区间删/两种 find） | `test_set()` |
| 第六节 | multiset（值冗余） | `test_multiset()` |
| 第七节 | map（pair/insert/operator[]/词频） | `test_map()` |
| 第八节 | multimap（一对多） | `test_multimap()` |
| 第九节 | OJ 实战：349 交集 / 142 环形链表 / 692 前 K 高频 | `test_oj()` |

## 手动编译（Linux / macOS / 有 g++ 的环境）

```bash
g++ -std=c++11 -O2 -Wall -Wextra bst_mapset_sim.cpp -o bst_mapset_sim && ./bst_mapset_sim
```

## 说明

- 实测环境：w64devkit g++ 15.2.0（`-std=c++11` / `-O2`）/ VS2022 x64
- `output.txt` 由 `build.bat` 真实运行生成
- 参考实现里的 KV 模型在"删除有两个孩子的结点"时只复制 key 的 bug，本仓库用 `Erase(key, false/true)` 做了错对对照实测

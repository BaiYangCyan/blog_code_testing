# unordered-sim：unordered_map / unordered_set 使用与性能实验

配套文章：《C++进阶篇（七）：unordered_map 和 unordered_set——哈希桶、负载因子与性能对比》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/待发布

## 一分钟上手

1. **只看结果**：打开 `output.txt`（unordered_set/map、bucket、multiset、100 万性能全在里面）
2. **自己跑**：双击 `build.bat`（需要 g++）
3. **看代码**：`unordered_sim.cpp` 按文章节次分块

## 本次实测摘要

```text
unordered_set：Size=4，count(3)=1，count(9)=0
排序后: 1 2 3 5（遍历顺序不保证）
erase(3) 后 Size=3

unordered_map：重复插入 left 不覆盖
词频: 苹果=6 西瓜=3 香蕉=2

bucket：100 个元素，bucket_count=167，load_factor=0.599
reserve(500)：bucket_count=761
rehash(1000)：bucket_count=1031

100 万数据：
  插入：set 436.65 ms，unordered_set 57.19 ms
  查找：set 524.05 ms，unordered_set 8.35 ms
  删除：set 475.60 ms，unordered_set 45.36 ms

rehash：bucket_count 127 → 541；原指针仍可读，原迭代器失效
```

## 编译

```bash
g++ -std=c++11 -O2 -Wall -Wextra unordered_sim.cpp -o unordered_sim && ./unordered_sim
```

## 说明

- 实测环境：w64devkit g++ 15.2.0（`-std=c++11 -O2 -Wall -Wextra`）/ VS2022 x64
- `output.txt` 由 `build.bat` 真实运行生成；时间数字随机器波动
- `bucket_count`、遍历顺序由标准库实现决定，不同平台可能不同
- 性能测试 N=1000000，两边数据、查找/删除顺序完全相同；unordered 先 `reserve(N)`
- 本篇只讲使用和性能，不实现哈希表；线性探测/链地址法/扩容留到下一篇
- 迭代器失效实验只读原指针，不解引用失效迭代器，避免未定义行为

# vector-sim：C++ vector 模拟实现配套代码

配套文章：《C++入门篇（十四）：vector 模拟实现——三指针、扩容流程与 memcpy 陷阱》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/details/166995456

## 文件说明

| 文件 | 对应文章位置 | 说明 |
| --- | --- | --- |
| `vector_sim.cpp` | 第一~八节 + 附录 | 完整教学版，包含全部测试，输出与文章一致 |
| `memcpy_crash.cpp` | 第七节 实验 A | memcpy 版 reserve + 自定义 string：堆损坏复现 |
| `memcpy_int_ok.cpp` | 第七节 实验 C | 同样的 memcpy 版 reserve + int：表面正常 |
| `growth.cpp` | 第十一节 互动实验 | 三条扩容曲线（2 倍 / 1.5 倍+最小增量 / 1.5 倍起点 4） |

## 编译运行（g++）

```bash
g++ -std=c++11 vector_sim.cpp    -o vector_sim    && ./vector_sim
g++ -std=c++11 memcpy_int_ok.cpp -o memcpy_int_ok && ./memcpy_int_ok
g++ -std=c++11 growth.cpp        -o growth        && ./growth
g++ -std=c++11 memcpy_crash.cpp  -o memcpy_crash  && ./memcpy_crash   # 会崩，属预期
```

## 预期输出

- `vector_sim`：依次打印各节测试结果，最后一行 `ALL TESTS PASSED`
- `memcpy_int_ok`：输出 `[memcpy + int] data=10 20 30 40 50 | size=5 capacity=8` 后正常退出
- `growth`：打印三条容量增长序列（1.5 倍最小增量那条与 VS 观察值一致）
- `memcpy_crash`：`push 1` ~ `push 5` 全部打印后，离开作用域、在析构阶段崩溃（Windows 退出码 `0xC0000374`，即 STATUS_HEAP_CORRUPTION）；编译时还会出现 `-Wclass-memaccess` 警告——这些都是实验 A 的预期现象，不是代码 bug

## 说明

- 全部为教学简化版，用于理解三指针、扩容流程、迭代器失效与 memcpy 陷阱，不代表标准库实现
- 生产代码请直接使用 `std::vector`
- 实测环境：w64devkit g++ 15.2.0（`-std=c++11`）/ VS2022 x64 Debug

# list-sim：C++ list 模拟实现配套代码

配套文章：《C++入门篇（十五）：list——带头双向循环链表：用法、迭代器失效与手写模拟实现》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/待发布

## 文件说明

| 文件 | 对应文章位置 | 说明 |
| --- | --- | --- |
| `list_sim.cpp` | 第五节 手写模拟实现 | 完整版：结点类 + 三参数模板迭代器 + 反向迭代器 + list 类，含全部测试 |
| `erase_bug_demo.cpp` | 第七节 常见错误对照 | 错误版 erase（不重连前后指针）vs 正确版，附安全自检输出 |
| `sort_speed.cpp` | 第六节 效率对比 | list.sort 与"拷贝到 vector 排序"的速度对比 |
| `misc_api.cpp` | 第三节 独门接口 | splice / merge / unique / remove / reverse / swap / std::find 实测 |
| `emplace_demo.cpp` | 第三节 3.1 | push_back vs emplace_back 的构造/拷贝/移动计数对比 |

## 编译运行（g++）

```bash
g++ -std=c++11 -Wall -Wextra list_sim.cpp    -o list_sim    && ./list_sim
g++ erase_bug_demo.cpp                       -o erase_bug_demo && ./erase_bug_demo
g++ -std=c++11 -O2 sort_speed.cpp            -o sort_speed  && ./sort_speed
g++ -std=c++11 -Wall -Wextra misc_api.cpp    -o misc_api    && ./misc_api
g++ -std=c++11 -Wall -Wextra emplace_demo.cpp -o emplace_demo && ./emplace_demo
```

> Windows PowerShell 5.1 不支持 `&&`，换成分号执行，或一行一行跑。

## 预期输出

- `list_sim`：依次打印正反向遍历、erase 循环、拷贝赋值、string 元素、const 打印测试，最后一行 `ALL TESTS PASSED`
- `erase_bug_demo`：错误版"双链自检：失败"，正确版"双链自检：通过，size=3，内容: 1 2 4"
- `sort_speed`：打印两种排序耗时（务必 -O2；Debug/无优化下结论会失真）
- `misc_api`：打印 splice / merge / unique 等接口的实测结果
- `emplace_demo`：打印三种插入方式的构造/拷贝/移动计数

## 说明

- 教学简化版，用于理解带头双向循环链表与迭代器封装，不代表标准库实现
- 生产代码请直接使用 `std::list`
- 实测环境：w64devkit g++ 15.2.0（`-std=c++11`）/ VS2022 x64

# inherit-sim：C++ 继承 一站式实验代码（单文件版）

配套文章：《C++进阶篇（一）：继承——权限、切片、默认成员函数与菱形继承》

文章链接：https://blog.csdn.net/Bai_YangSQ/article/待发布

## 一分钟上手

1. **只想看结果**：直接打开 `output.txt`（含 sizeof 实测、地址对比、构造析构顺序），不用装编译器
2. **想自己跑**：双击 `build.bat`（需要 g++），一键编译 + 运行 + 刷新 output.txt
3. **想找某段代码**：打开 `inherit_sim.cpp`，按下面导览表 Ctrl+F 搜函数名

## 代码导览

| 文章章节 | 内容 | 搜索函数名 |
| --- | --- | --- |
| 第一节 | 基础继承 + protected / private 可见性 | `test_inherit_basic()` |
| 第二节 | 继承类模板（stack : vector，需指定类域） | `test_inherit_template()` |
| 第三节 | 赋值兼容 + 对象切片 | `test_convert_slice()` |
| 第四节 | 作用域与隐藏（_num / fun） | `test_hiding()` |
| 第五节 | 默认成员函数全链路（构造 / 拷贝 / 赋值 / 析构顺序） | `test_default_members()` |
| 第五节附 | 不能被继承的类（final / 私有构造） | `test_final()` |
| 第六节 | 友元与静态成员（地址实测） | `test_friend_static()` |
| 第七节 | 多继承、菱形继承与虚继承（sizeof / 指针偏移实测） | `test_diamond()` |
| 第八节 | 继承与组合（is-a / has-a） | `test_compose()` |

## 手动编译（Linux / macOS / 有 g++ 的环境）

```bash
g++ -std=c++11 -O2 -Wall -Wextra inherit_sim.cpp -o inherit_sim && ./inherit_sim
```

## 说明

- 实测环境：w64devkit g++ 15.2.0（`-std=c++11` / `-O2`）/ VS2022 x64
- `output.txt` 由 `build.bat` 真实运行生成
- sizeof / 地址 / 偏移等数据与编译器和平台有关，本仓库为 x64 实测值

// ============================================================================
// 模板进阶 一站式实验（单文件版）：非类型模板参数 + 特化全家桶
// 配套文章：《C++入门篇（十八）：模板进阶——非类型参数、特化全家桶与分离编译》
//
// 顶部导航（在编辑器里 Ctrl+F 搜函数名，直达文章对应节）：
//   第一节  非类型模板参数（array<T, N>）          → namespace bite + test_array()
//   第三节  函数模板特化（Less<Date*>）            → test_func_spec()
//   第四节  类模板全特化 / 偏特化（Data 家族）      → test_class_spec()
//   第五节  类模板特化实战（排序 Date*）            → test_sort_ptr()
//   分离编译  见 split/ 子目录（坏例 / 修复例 / 显式实例化）
//
// 编译运行：
//   g++ -std=c++11 -O2 -Wall -Wextra template_advanced_sim.cpp -o template_advanced_sim && ./template_advanced_sim
// Windows 用户直接双击 build.bat（一键编译 + 运行 + 刷新 output.txt，含 split 演示）
// ============================================================================
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstddef>
using namespace std;

// ============================================================================
// 一、非类型模板参数：array<T, N>
// ============================================================================
namespace bite
{
// N 是"非类型模板参数"：常量当参数用，编译期确定数组大小
template <class T, size_t N = 10>
class array
{
public:
    array() : _size(N) {}   // 原版示例没给 _size 赋值，这里补上让它能跑

    T& operator[](size_t index) { return _array[index]; }
    const T& operator[](size_t index) const { return _array[index]; }

    size_t size() const { return _size; }
    bool empty() const { return 0 == _size; }

private:
    T _array[N];
    size_t _size;
};
}

void test_array()
{
    bite::array<int, 5> a5;    // N = 5
    bite::array<int> a10;      // 用默认值 N = 10

    for (size_t i = 0; i < a5.size(); ++i)
        a5[i] = (int)(i * 10);

    cout << "array<int, 5>  size=" << a5.size() << "  data: ";
    for (size_t i = 0; i < a5.size(); ++i)
        cout << a5[i] << " ";
    cout << endl;

    cout << "array<int>     size=" << a10.size() << "（默认参数 N=10）" << endl;
}

// ============================================================================
// 二、函数模板特化用的 Date（同时供第五节排序实战使用）
// ============================================================================
class Date
{
public:
    Date(int year = 1900, int month = 1, int day = 1)
        : _year(year), _month(month), _day(day)
    {}

    bool operator<(const Date& d) const
    {
        return (_year < d._year) ||
               (_year == d._year && _month < d._month) ||
               (_year == d._year && _month == d._month && _day < d._day);
    }

    friend ostream& operator<<(ostream& _cout, const Date& d);

    int _year, _month, _day;
};

ostream& operator<<(ostream& _cout, const Date& d)
{
    _cout << d._year << "-" << d._month << "-" << d._day;
    return _cout;
}

// ============================================================================
// 三、函数模板特化
// ============================================================================
namespace bite
{
// 基础函数模板：大多数类型都能比，但指针会"比错"
// （课件里这个函数也叫 Less；因为和后面第五节的特化仿函数类重名，
//   单文件里按课堂代码的命名改成 LessFunc，功能完全一致）
template <class T>
bool LessFunc(T left, T right)
{
    return left < right;
}

// 对 LessFunc 函数模板进行特化：指针类型改为比较"指向的对象"
template <>
bool LessFunc<Date*>(Date* left, Date* right)
{
    return *left < *right;
}
}

void test_func_spec()
{
    // 数组元素的地址随下标递增：故意让"地址顺序"和"日期顺序"拧着来
    Date arr[2] = {Date(2022, 7, 8), Date(2022, 7, 7)};
    Date* p1 = &arr[0];   // 日期更大，地址更低
    Date* p2 = &arr[1];   // 日期更小，地址更高

    cout << "LessFunc(1, 2)         = " << bite::LessFunc(1, 2) << "（内置类型，正确）" << endl;
    cout << "LessFunc(d1, d2)       = " << bite::LessFunc(Date(2022, 7, 7), Date(2022, 7, 8))
         << "（日期对象，正确）" << endl;
    cout << "LessFunc(p1, p2)       = " << bite::LessFunc(p1, p2)
         << "（特化后比对象：7-8 < 7-7 不成立，正确）" << endl;
    cout << "std::less<Date*>()     = " << std::less<Date*>()(p1, p2)
         << "（未特化：比地址，错误）" << endl;
}

// ============================================================================
// 四、类模板全特化 / 偏特化：Data 家族
// ============================================================================
// 主模板
template <class T1, class T2>
class Data
{
public:
    Data() { cout << "Data<T1, T2>（主模板）" << endl; }
private:
    T1 _d1;
    T2 _d2;
};

// 全特化：所有参数都定死
template <>
class Data<int, char>
{
public:
    Data() { cout << "Data<int, char>（全特化）" << endl; }
private:
    int _d1;
    char _d2;
};

// 偏特化一：部分参数定死（第二个参数是 int）
template <class T1>
class Data<T1, int>
{
public:
    Data() { cout << "Data<T1, int>（偏特化）" << endl; }
private:
    T1 _d1;
    int _d2;
};

// 偏特化二：参数更进一步限制——两个都是指针
template <typename T1, typename T2>
class Data<T1*, T2*>
{
public:
    Data() { cout << "Data<T1*, T2*>（指针偏特化）" << endl; }
private:
    T1 _d1;
    T2 _d2;
};

// 偏特化三：两个都是引用
template <typename T1, typename T2>
class Data<T1&, T2&>
{
public:
    Data(const T1& d1, const T2& d2)
        : _d1(d1), _d2(d2)
    {
        cout << "Data<T1&, T2&>（引用偏特化）" << endl;
    }
private:
    const T1& _d1;
    const T2& _d2;
};

void test_class_spec()
{
    Data<int, int>    d1;         // 命中 Data<T1, int>
    Data<int, char>   d2;         // 命中全特化 Data<int, char>
    Data<double, int> d3;         // 命中 Data<T1, int>
    Data<int, double> d4;         // 都不命中 → 主模板
    Data<int*, int*>  d5;         // 命中指针偏特化

    int x = 1, y = 2;
    Data<int&, int&>  d6(x, y);   // 命中引用偏特化
    (void)d1; (void)d2; (void)d3; (void)d4; (void)d5; (void)d6;
}

// ============================================================================
// 五、类模板特化实战：给 vector<Date*> 排序
// ============================================================================
namespace bite
{
// 仿函数类模板：默认按 < 比较
template <class T>
struct Less
{
    bool operator()(const T& x, const T& y) const { return x < y; }
};

// 全特化：Date* 改为比较指向的日期对象
template <>
struct Less<Date*>
{
    bool operator()(Date* x, Date* y) const { return *x < *y; }
};
}

template <class T>
void print_vec(const char* tag, const vector<T>& v)
{
    cout << tag;
    for (const auto& e : v) cout << e << " ";
    cout << endl;
}

// 指针版本：解引用打印，看清"按地址排序"到底排出了什么日期
void print_vec_ptr(const char* tag, const vector<Date*>& v)
{
    cout << tag;
    for (const auto& e : v) cout << *e << " ";
    cout << endl;
}

void test_sort_ptr()
{
    // 数组元素地址随下标递增：7-7、7-6、7-8
    Date arr[3] = {Date(2022, 7, 7), Date(2022, 7, 6), Date(2022, 7, 8)};

    // 1. vector<Date>：直接用 Less<Date> 排，正确
    vector<Date> v1{arr[0], arr[1], arr[2]};
    sort(v1.begin(), v1.end(), bite::Less<Date>());
    print_vec("[vector<Date>  排序] ", v1);

    // 2. vector<Date*>：指针乱序入队（7-8、7-7、7-6）
    vector<Date*> v2{&arr[2], &arr[0], &arr[1]};

    // 用"未特化"的比较器（按地址）→ 排出来是地址顺序，不是日期顺序
    vector<Date*> v2b(v2);
    sort(v2b.begin(), v2b.end(), std::less<Date*>());
    print_vec_ptr("[按地址排序  ] ", v2b);

    // 用全特化后的 bite::Less<Date*> → 正确按日期
    sort(v2.begin(), v2.end(), bite::Less<Date*>());
    print_vec_ptr("[Less<Date*> 特化] ", v2);
}

// ============================================================================
// main：按文章节次顺序跑全部实验
// ============================================================================
int main()
{
    cout << "========== 第一节 非类型模板参数 ==========" << endl;
    test_array();

    cout << endl << "========== 第三节 函数模板特化 ==========" << endl;
    test_func_spec();

    cout << endl << "========== 第四节 类模板全特化 / 偏特化 ==========" << endl;
    test_class_spec();

    cout << endl << "========== 第五节 类模板特化实战：排序 Date* ==========" << endl;
    test_sort_ptr();

    return 0;
}

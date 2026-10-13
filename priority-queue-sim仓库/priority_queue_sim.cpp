// ============================================================================
// priority_queue 完整模拟实现 + 仿函数专题（单文件版）
// 配套文章：《C++入门篇（十七）：priority_queue 完整模拟实现——仿函数、区间建堆与指针特化》
//
// 顶部导航（在编辑器里 Ctrl+F 搜函数名，直达文章对应节）：
//   第二节  仿函数（函数对象）与算法联动        → test_functor()
//   第三节  完整模拟实现（构造建堆 + 上下调整）  → namespace bit + test_build_heap()
//   第四节  自定义类型与指针陷阱                → test_custom_type() / test_pointer_trap()
//   第五节  Top-K 实战                          → test_topk()
//   附       deque 排序效率实测（需 -O2）       → test_deque_sort()
//
// 编译运行（建议 -O2）：
//   g++ -std=c++11 -O2 -Wall -Wextra priority_queue_sim.cpp -o priority_queue_sim && ./priority_queue_sim
// Windows 用户直接双击 build.bat（一键编译 + 运行 + 刷新 output.txt）
// ============================================================================
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <ctime>
using namespace std;

// ============================================================================
// 一、仿函数库（Less / Greater）+ priority_queue 完整实现
// ============================================================================
namespace bit
{
// ---------------- 1. 仿函数：对象像函数一样用 ----------------
template <class T>
struct Less
{
    bool operator()(const T& x, const T& y) { return x < y; }
};

template <class T>
struct Greater
{
    bool operator()(const T& x, const T& y) { return x > y; }
};

// 偏特化：所有"指针类型"都按指向的对象比较（解决 Date* 默认比地址的坑）
template <class T>
struct Less<T*>
{
    bool operator()(T* const& x, T* const& y) { return *x < *y; }
};

// ---------------- 2. priority_queue：容器适配器 ----------------
// T 决定元素类型；Container 决定底层容器；Compare 单独抽出来"解耦"比较逻辑
template <class T, class Container = vector<T>, class Compare = Less<T>>
class priority_queue
{
public:
    priority_queue() {}

    // 迭代器区间构造：先装进底层容器，再自底向上建堆
    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last)
        : _con(first, last)
    {
        // 建堆：从最后一个非叶子结点倒着向下调整
        for (int i = ((int)_con.size() - 2) / 2; i >= 0; --i)
            adjust_down((size_t)i);
    }

    void push(const T& x)
    {
        _con.push_back(x);
        adjust_up(_con.size() - 1);
    }

    void pop()
    {
        swap(_con[0], _con[_con.size() - 1]);
        _con.pop_back();
        adjust_down(0);
    }

    const T& top() const { return _con[0]; }
    bool empty() const   { return _con.empty(); }
    size_t size() const  { return _con.size(); }

private:
    // 向上调整：新元素从尾部浮到该去的位置
    void adjust_up(size_t child)
    {
        size_t parent = (child - 1) / 2;
        while (child > 0)
        {
            Compare compare;   // 仿函数对象：像函数一样调用
            if (compare(_con[parent], _con[child]))
            {
                swap(_con[child], _con[parent]);
                child = parent;
                parent = (child - 1) / 2;
            }
            else
                break;
        }
    }

    // 向下调整：堆顶下沉到该去的位置
    void adjust_down(size_t parent)
    {
        size_t child = parent * 2 + 1;
        Compare compare;
        while (child < _con.size())
        {
            if (child + 1 < _con.size() && compare(_con[child], _con[child + 1]))
                ++child;   // 挑"更该上去"的那个孩子

            if (compare(_con[parent], _con[child]))
            {
                swap(_con[child], _con[parent]);
                parent = child;
                child = parent * 2 + 1;
            }
            else
                break;
        }
    }

    Container _con;
};

} // namespace bit

// ============================================================================
// 二、第二节：仿函数（函数对象）与算法联动
// ============================================================================
// 条件仿函数：既是奇数、又是 11 的倍数
class Conditional
{
public:
    bool operator()(int x) { return x % 2 && x % 11 == 0; }
};

void test_functor()
{
    // 1. 调用本质：lessFunc(1, 2) 等价于 lessFunc.operator()(1, 2)
    bit::Less<int> lessFunc;
    bit::Greater<int> greaterFunc;
    cout << "lessFunc(1, 2) = " << lessFunc(1, 2)
         << "，lessFunc.operator()(1, 2) = " << lessFunc.operator()(1, 2) << endl;
    cout << "greaterFunc(1, 2) = " << greaterFunc(1, 2) << endl;

    // 2. 仿函数给算法用：sort 降序
    vector<int> v{3, 2, 7, 6, 0, 4, 1, 9, 8, 5, 2, 2, 3};
    sort(v.begin(), v.end(), bit::Greater<int>());
    cout << "sort 降序: ";
    for (auto e : v) cout << e << " ";
    cout << endl;

    // 3. 仿函数当"条件"用：find_if
    vector<int> myvector{10, 25, 40, 55};
    auto it = find_if(myvector.begin(), myvector.end(), Conditional());
    cout << "find_if 找到第一个奇数且 11 的倍数: " << *it << endl;
}

// ============================================================================
// 三、第三节：完整模拟实现实测（建堆与调整）
// ============================================================================
void test_build_heap()
{
    vector<int> v{3, 2, 7, 6, 0, 4, 1, 9, 8, 5, 2, 2, 3};

    // 1. 区间构造（内部建堆）→ 大堆
    bit::priority_queue<int> q1(v.begin(), v.end());
    cout << "[大堆·区间建堆] top=" << q1.top() << " | 弹 5 次: ";
    for (int i = 0; i < 5 && !q1.empty(); ++i)
    {
        cout << q1.top() << " ";
        q1.pop();
    }
    cout << endl;

    // 2. 逐个 push → 大堆（结果应与区间建堆一致）
    bit::priority_queue<int> q2;
    for (auto e : v) q2.push(e);
    cout << "[大堆·逐个push] top=" << q2.top() << " | 弹 5 次: ";
    for (int i = 0; i < 5 && !q2.empty(); ++i)
    {
        cout << q2.top() << " ";
        q2.pop();
    }
    cout << endl;

    // 3. 换 Greater → 小堆
    bit::priority_queue<int, vector<int>, bit::Greater<int>> q3(v.begin(), v.end());
    cout << "[小堆·区间建堆] top=" << q3.top() << " | 弹 5 次: ";
    for (int i = 0; i < 5 && !q3.empty(); ++i)
    {
        cout << q3.top() << " ";
        q3.pop();
    }
    cout << endl;
}

// ============================================================================
// 四、第四节：自定义类型与指针陷阱
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

    bool operator>(const Date& d) const
    {
        return (_year > d._year) ||
               (_year == d._year && _month > d._month) ||
               (_year == d._year && _month == d._month && _day > d._day);
    }

    friend ostream& operator<<(ostream& _cout, const Date& d);

    // 教学方便起见成员公开；工程里建议设为 private + 提供 operator< 等接口
    int _year;
    int _month;
    int _day;
};

ostream& operator<<(ostream& _cout, const Date& d)
{
    _cout << d._year << "-" << d._month << "-" << d._day;
    return _cout;
}

// 自定义仿函数：为 Date 量身定做比较逻辑（不依赖 Date 自身的运算符也能用）
class LessDate
{
public:
    bool operator()(const Date& x, const Date& y)
    {
        return (x._year < y._year) ||
               (x._year == y._year && x._month < y._month) ||
               (x._year == y._year && x._month == y._month && x._day < y._day);
    }
};

void test_custom_type()
{
    // 1. 自定义仿函数控制比较：大堆
    bit::priority_queue<Date, vector<Date>, LessDate> q1;
    q1.push(Date(2026, 10, 29));
    q1.push(Date(2026, 10, 28));
    q1.push(Date(2026, 10, 30));
    q1.push(Date(2020, 1, 30));
    q1.push(Date(2021, 3, 30));

    cout << "[LessDate 大堆] top=" << q1.top() << " | 依次弹出: ";
    while (!q1.empty())
    {
        cout << q1.top() << " ";
        q1.pop();
    }
    cout << endl;

    // 2. 用 Date::operator> + Greater 仿函数：小堆
    bit::priority_queue<Date, vector<Date>, bit::Greater<Date>> q2;
    q2.push(Date(2026, 10, 29));
    q2.push(Date(2026, 10, 30));
    q2.push(Date(2026, 10, 28));

    cout << "[Greater 小堆] top=" << q2.top() << " | 弹 3 次: ";
    for (int i = 0; i < 3 && !q2.empty(); ++i)
    {
        cout << q2.top() << " ";
        q2.pop();
    }
    cout << endl;
}

void test_pointer_trap()
{
    // arr 内地址随下标递增（C++ 保证数组元素地址连续递增）
    // 故意让"地址顺序"和"日期顺序"相反
    Date arr[2] = {Date(2026, 10, 30), Date(2026, 10, 29)};
    Date* p30 = &arr[0];   // 日期更大，地址更低
    Date* p29 = &arr[1];   // 日期更小，地址更高

    cout << "按地址比：std::less<Date*>()(p30, p29) = " << std::less<Date*>()(p30, p29)
         << "   ← 竟认为 10-30 < 10-29（错）" << endl;
    cout << "按对象比：bit::Less<Date*>()(p30, p29)  = " << bit::Less<Date*>()(p30, p29)
         << "   ← 偏特化后比的是日期（对）" << endl;

    vector<Date*> vp{p30, p29};

    // 未特化的比较器：按指针地址排
    bit::priority_queue<Date*, vector<Date*>, std::less<Date*>> q_bad(vp.begin(), vp.end());
    cout << "[std::less 版本] top=" << *q_bad.top() << "   ← 比地址，拿到错的堆顶" << endl;

    // bit::Less<T*> 偏特化后：按指向的日期排
    bit::priority_queue<Date*> q_good(vp.begin(), vp.end());
    cout << "[bit::Less 特化版] top=" << *q_good.top() << "   ← 按日期，正确" << endl;
}

// ============================================================================
// 五、第五节：Top-K 实战
// ============================================================================
void test_topk()
{
    vector<int> v{3, 2, 7, 6, 0, 4, 1, 9, 8, 5, 2, 2, 3};

    // 前 K 个最大：大堆弹 K 次（OJ 里 K 用 cin 读入）
    const int k = 5;
    bit::priority_queue<int> q1(v.begin(), v.end());
    cout << "前 " << k << " 大: ";
    for (int i = 0; i < k && !q1.empty(); ++i)
    {
        cout << q1.top() << " ";
        q1.pop();
    }
    cout << endl;

    // 第 K 大：大堆弹 K-1 次，剩下的堆顶就是答案（演示 K=3）
    const int kk = 3;
    bit::priority_queue<int> q2(v.begin(), v.end());
    for (int i = 0; i < kk - 1; ++i)
        q2.pop();
    cout << "第 " << kk << " 大: " << q2.top() << endl;

    // 前 K 个最小：小堆弹 K 次
    bit::priority_queue<int, vector<int>, bit::Greater<int>> q3(v.begin(), v.end());
    cout << "前 " << k << " 小: ";
    for (int i = 0; i < k && !q3.empty(); ++i)
    {
        cout << q3.top() << " ";
        q3.pop();
    }
    cout << endl;
}

// ============================================================================
// 六、附：deque 排序效率实测（结论：拷到 vector 排完再拷回反而更快）
// ============================================================================
void test_deque_sort()
{
    srand(1);
    const int N = 1000000;

    deque<int> dq1, dq2;
    for (int i = 0; i < N; ++i)
    {
        int e = rand() + i;
        dq1.push_back(e);
        dq2.push_back(e);
    }

    clock_t b1 = clock();
    sort(dq1.begin(), dq1.end());          // 直接在 deque 上排
    clock_t e1 = clock();

    clock_t b2 = clock();
    vector<int> v(dq2.begin(), dq2.end()); // 拷贝到 vector
    sort(v.begin(), v.end());
    dq2.assign(v.begin(), v.end());        // 再拷回
    clock_t e2 = clock();

    cout << "N = " << N << "（-O2 实测）" << endl;
    cout << "直接 sort(deque)              : " << (e1 - b1) * 1000.0 / CLOCKS_PER_SEC << " ms" << endl;
    cout << "拷贝到 vector 排序再拷回 deque : " << (e2 - b2) * 1000.0 / CLOCKS_PER_SEC << " ms" << endl;
}

// ============================================================================
// main：按文章节次顺序跑全部实验
// ============================================================================
int main()
{
    cout << "========== 第二节 仿函数（函数对象） ==========" << endl;
    test_functor();

    cout << endl << "========== 第三节 完整模拟实现：建堆与调整 ==========" << endl;
    test_build_heap();

    cout << endl << "========== 第四节 自定义类型与指针陷阱 ==========" << endl;
    test_custom_type();
    cout << endl;
    test_pointer_trap();

    cout << endl << "========== 第五节 Top-K 实战 ==========" << endl;
    test_topk();

    cout << endl << "========== 附：deque 排序效率实测 ==========" << endl;
    test_deque_sort();

    return 0;
}

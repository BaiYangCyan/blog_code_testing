// ============================================================================
// stack / queue / priority_queue 一站式实验（单文件版）
// 配套文章：《C++入门篇（十六）：stack 和 queue——容器适配器、deque 底层与手写模拟实现》
//
// 顶部导航（在编辑器里 Ctrl+F 搜函数名，直达文章对应节）：
//   第二节  stack 实战：最小栈（OJ）          → class MinStack + test_min_stack()
//   第四节  容器适配器：同一套壳换底层        → test_adapter_switch()
//   第五节  手写模拟实现（stack / queue）     → namespace bit + test_stack_all() / test_queue_all()
//   第六节  priority_queue 手写堆（大堆小堆） → namespace bit + test_priority_queue()
//   第七节  deque 特性验证                    → test_deque()
//
// 编译运行：
//   g++ -std=c++11 -O2 -Wall -Wextra stack_queue_sim.cpp -o stack_queue_sim && ./stack_queue_sim
// Windows 用户直接双击 build.bat（一键编译 + 运行 + 刷新 output.txt）
// ============================================================================
#include <iostream>
#include <deque>
#include <vector>
#include <list>
#include <stack>
using namespace std;

namespace bit
{
// ============================================================================
// 一、stack 适配器（第五节 5.1）
// ============================================================================
template <class T, class Con = deque<T>>
class stack
{
public:
    stack() {}

    void push(const T& x) { _c.push_back(x); }
    void pop()            { _c.pop_back(); }
    T& top()              { return _c.back(); }
    const T& top() const  { return _c.back(); }
    size_t size() const   { return _c.size(); }
    bool empty() const    { return _c.empty(); }

private:
    Con _c;
};

// ============================================================================
// 二、queue 适配器（第五节 5.2）
// ============================================================================
template <class T, class Con = deque<T>>
class queue
{
public:
    queue() {}

    void push(const T& x) { _c.push_back(x); }
    void pop()            { _c.pop_front(); }
    T& front()            { return _c.front(); }
    const T& front() const { return _c.front(); }
    T& back()             { return _c.back(); }
    const T& back() const { return _c.back(); }
    size_t size() const   { return _c.size(); }
    bool empty() const    { return _c.empty(); }

private:
    Con _c;
};

// ============================================================================
// 三、仿函数 + priority_queue（第六节 6.2）
// ============================================================================
template <class T>
struct less
{
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <class T>
struct greater
{
    bool operator()(const T& a, const T& b) const { return a > b; }
};

template <class T, class Con = vector<T>, class Compare = less<T>>
class priority_queue
{
public:
    priority_queue() {}

    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last)
    {
        while (first != last)
        {
            _c.push_back(*first);
            ++first;
        }
        // 建堆：从最后一个非叶子结点开始，依次向下调整
        int i = ((int)_c.size() - 2) / 2;
        for (; i >= 0; --i)
            adjust_down(i);
    }

    void push(const T& x)
    {
        _c.push_back(x);
        adjust_up(_c.size() - 1);
    }

    void pop()
    {
        swap(_c[0], _c[_c.size() - 1]);
        _c.pop_back();
        adjust_down(0);
    }

    const T& top() const { return _c[0]; }
    size_t size() const  { return _c.size(); }
    bool empty() const   { return _c.empty(); }

private:
    void adjust_up(size_t child)
    {
        Compare com;
        size_t parent = (child - 1) / 2;
        while (child > 0)
        {
            if (com(_c[parent], _c[child]))     // 双亲比孩子"小"，交换
            {
                swap(_c[parent], _c[child]);
                child = parent;
                parent = (child - 1) / 2;
            }
            else
                break;
        }
    }

    void adjust_down(size_t parent)
    {
        Compare com;
        size_t child = parent * 2 + 1;
        while (child < _c.size())
        {
            if (child + 1 < _c.size() && com(_c[child], _c[child + 1]))
                ++child;                        // 挑"更该上去"的那个孩子
            if (com(_c[parent], _c[child]))
            {
                swap(_c[parent], _c[child]);
                parent = child;
                child = parent * 2 + 1;
            }
            else
                break;
        }
    }

    Con _c;
};

} // namespace bit

// ============================================================================
// 四、第二节：最小栈（OJ）——push / pop / top / getMin 全 O(1)
// ============================================================================
class MinStack
{
public:
    void push(int x)
    {
        _elem.push(x);
        // 只有"不大于当前最小"才记入 _min（重复最小值也要记，pop 才不会错）
        if (_min.empty() || x <= _min.top())
            _min.push(x);
    }

    void pop()
    {
        // 出栈的元素正好是当前最小，_min 跟着出
        if (_min.top() == _elem.top())
            _min.pop();
        _elem.pop();
    }

    int top()    { return _elem.top(); }
    int getMin() { return _min.top(); }

private:
    stack<int> _elem;   // 存所有元素
    stack<int> _min;    // 存"当前最小值"的历史
};

void test_min_stack()
{
    MinStack st;
    st.push(-2);
    st.push(0);
    st.push(-3);
    cout << "getMin=" << st.getMin() << endl;                    // -3

    st.pop();
    cout << "top=" << st.top() << " getMin=" << st.getMin() << endl;  // 0  -2

    st.pop();
    cout << "top=" << st.top() << " getMin=" << st.getMin() << endl;  // -2 -2
}

// ============================================================================
// 五、第四节：容器适配器换底层（行为一致，差别只在性能）
// ============================================================================
void test_adapter_switch()
{
    // 同一套接口，三种底层
    bit::stack<int>                  s1;   // 默认 deque
    bit::stack<int, vector<int>>     s2;   // 换 vector（stack 只需要尾插尾删）
    bit::stack<int, list<int>>       s3;   // 换 list

    for (int i = 1; i <= 4; ++i)
    {
        s1.push(i); s2.push(i); s3.push(i);
    }
    cout << "stack 顶: " << s1.top() << " " << s2.top() << " " << s3.top() << endl;

    // queue 必须有头删，只能 deque / list
    bit::queue<int>              q1;       // 默认 deque
    bit::queue<int, list<int>>   q2;       // 换 list
    for (int i = 1; i <= 4; ++i)
    {
        q1.push(i); q2.push(i);
    }
    cout << "queue 头尾: " << q1.front() << "/" << q1.back()
         << "  " << q2.front() << "/" << q2.back() << endl;
}

// ============================================================================
// 六、第五节：手写模拟实现全部测试
// ============================================================================
template <class St>
void test_stack(const char* tag)
{
    St s;
    for (int i = 1; i <= 5; ++i)
        s.push(i * 10);

    cout << tag << "size=" << s.size() << " top=" << s.top() << " | pop: ";
    while (!s.empty())
    {
        cout << s.top() << " ";
        s.pop();
    }
    cout << "| empty=" << (s.empty() ? 1 : 0) << endl;
}

void test_stack_all()
{
    test_stack<bit::stack<int>>("[deque 底层] ");
    test_stack<bit::stack<int, vector<int>>>("[vector 底层] ");
    test_stack<bit::stack<int, list<int>>>("[list 底层] ");
}

template <class Q>
void test_queue(const char* tag)
{
    Q q;
    for (int i = 1; i <= 5; ++i)
        q.push(i * 10);

    cout << tag << "size=" << q.size() << " front=" << q.front() << " back=" << q.back() << " | pop: ";
    while (!q.empty())
    {
        cout << q.front() << " ";
        q.pop();
    }
    cout << "| empty=" << (q.empty() ? 1 : 0) << endl;
}

void test_queue_all()
{
    test_queue<bit::queue<int>>("[deque 底层] ");
    test_queue<bit::queue<int, list<int>>>("[list 底层] ");
}

// ============================================================================
// 七、第六节：priority_queue 手写堆（默认大堆 / greater 小堆）
// ============================================================================
template <class PQ>
void test_pq(const char* tag, const vector<int>& v)
{
    PQ pq(v.begin(), v.end());

    cout << tag << "top=" << pq.top() << " | 依次弹出: ";
    while (!pq.empty())
    {
        cout << pq.top() << " ";
        pq.pop();
    }
    cout << endl;
}

void test_priority_queue()
{
    vector<int> v{3, 2, 7, 6, 0, 4, 1, 9, 8, 5};

    test_pq<bit::priority_queue<int>>("[默认大堆] ", v);
    test_pq<bit::priority_queue<int, vector<int>, bit::greater<int>>>("[greater 小堆] ", v);

    // 逐个 push 的路径也测一遍
    bit::priority_queue<int> pq;
    for (int e : v) pq.push(e);
    cout << "[push 版大堆] size=" << pq.size() << " top=" << pq.top() << endl;
}

// ============================================================================
// 八、第七节：deque 特性验证（双端 O(1) + 支持随机访问）
// ============================================================================
template <class T>
void print_deque(const char* tag, const deque<T>& dq)
{
    cout << tag;
    for (const auto& e : dq) cout << e << " ";
    cout << "| size=" << dq.size() << endl;
}

void test_deque()
{
    deque<int> dq;

    // 尾插 + 头插：都是 O(1)
    for (int i = 1; i <= 3; ++i) dq.push_back(i * 10);
    dq.push_front(0);
    dq.push_front(-10);
    print_deque("双端插入后: ", dq);          // -10 0 10 20 30

    // 随机访问：和 vector 一样支持 []
    cout << "dq[0]=" << dq[0] << "  dq[4]=" << dq[4] << endl;

    // 中间插入：要搬元素，别多用
    dq.insert(dq.begin() + 3, 99);
    print_deque("中间插 99: ", dq);           // -10 0 10 99 20 30

    // 双端删除：也是 O(1)
    dq.pop_front();
    dq.pop_back();
    print_deque("两头各删一个: ", dq);        // 0 10 99 20
}

// ============================================================================
// main：按文章节次顺序跑全部实验
// ============================================================================
int main()
{
    cout << "========== 第二节 stack 实战：最小栈 ==========" << endl;
    test_min_stack();

    cout << endl << "========== 第四节 容器适配器：换底层 ==========" << endl;
    test_adapter_switch();

    cout << endl << "========== 第五节 手写模拟实现测试 ==========" << endl;
    test_stack_all();
    test_queue_all();

    cout << endl << "========== 第六节 priority_queue 手写堆 ==========" << endl;
    test_priority_queue();

    cout << endl << "========== 第七节 deque 特性验证 ==========" << endl;
    test_deque();

    return 0;
}

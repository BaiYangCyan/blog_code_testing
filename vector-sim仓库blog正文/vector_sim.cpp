// ============================================================================
// vector 模拟实现 + 全部实验（单文件版）
// 配套文章：《C++入门篇（十四）：vector 模拟实现——三指针、扩容流程与 memcpy 陷阱》
//
// 顶部导航（在编辑器里 Ctrl+F 搜函数名，直达文章对应节）：
//   第一~六节 + 第七节实验 B  手写 vector 全部测试（安全）  → namespace bit + test_sim()
//   第七节 实验 C             memcpy + int 对照组（表面正常）→ bitec::vector + test_memcpy_int()
//   第七节 实验 A             memcpy + 自定义类型（会崩溃！）→ test_memcpy_crash()（需 -DRUN_CRASH）
//   第十一节                  扩容曲线对比                    → test_growth()
//
// 编译运行（默认跳过崩溃实验）：
//   g++ -std=c++11 -O2 -Wall -Wextra vector_sim.cpp -o vector_sim && ./vector_sim
//
// 复现崩溃实验（观察 Windows 退出码 0xC0000374 堆损坏）：
//   g++ -std=c++11 -O2 -DRUN_CRASH vector_sim.cpp -o vector_crash && ./vector_crash
//
// Windows 用户直接双击 build.bat（一键编译 + 运行 + 刷新 output.txt）
// ============================================================================
#include <iostream>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <vector>
#include <cstdlib>
using namespace std;

// ============================================================================
// 一、bit::vector —— 手写模拟实现（正确版：逐个深拷贝）
// ============================================================================
namespace bit
{
template <class T>
class vector
{
public:
    typedef T* iterator;
    typedef const T* const_iterator;

    vector()
        : _start(nullptr), _finish(nullptr), _endofstorage(nullptr)
    {}

    vector(size_t n, const T& val = T())
    {
        _start = new T[n];
        for (size_t i = 0; i < n; ++i)
            _start[i] = val;
        _finish = _start + n;
        _endofstorage = _finish;
    }

    vector(const vector<T>& v)
    {
        _start = new T[v.capacity()];
        for (size_t i = 0; i < v.size(); ++i)
            _start[i] = v[i];
        _finish = _start + v.size();
        _endofstorage = _start + v.capacity();
    }

    vector<T>& operator=(vector<T> v)     // 值传递 + swap
    {
        swap(v);
        return *this;
    }

    ~vector()
    {
        delete[] _start;
        _start = _finish = _endofstorage = nullptr;
    }

    iterator begin() { return _start; }
    iterator end()   { return _finish; }
    const_iterator begin() const { return _start; }
    const_iterator end() const   { return _finish; }

    size_t size() const     { return _finish - _start; }
    size_t capacity() const { return _endofstorage - _start; }
    bool empty() const      { return _finish == _start; }

    T& operator[](size_t i)             { return _start[i]; }
    const T& operator[](size_t i) const { return _start[i]; }

    void reserve(size_t n)
    {
        if (n > capacity())
        {
            size_t sz = size();
            T* tmp = new T[n];
            for (size_t i = 0; i < sz; ++i)
                tmp[i] = _start[i];       // 逐个深拷贝，绝不能用 memcpy
            delete[] _start;
            _start = tmp;
            _finish = _start + sz;
            _endofstorage = _start + n;
        }
    }

    void push_back(const T& x)
    {
        if (_finish == _endofstorage)
        {
            size_t newcap = capacity() == 0 ? 4 : capacity() * 2;
            reserve(newcap);
        }
        *_finish = x;
        ++_finish;
    }

    void pop_back()
    {
        assert(!empty());
        --_finish;
    }

    void resize(size_t n, const T& val = T())
    {
        if (n < size())
        {
            _finish = _start + n;
        }
        else
        {
            if (n > capacity())
                reserve(n);
            while (_finish < _start + n)
            {
                *_finish = val;
                ++_finish;
            }
        }
    }

    iterator insert(iterator pos, const T& x)
    {
        assert(pos >= _start && pos <= _finish);
        if (_finish == _endofstorage)
        {
            size_t len = pos - _start;    // 先记相对位置
            reserve(capacity() == 0 ? 4 : capacity() * 2);
            pos = _start + len;           // 扩容后重新定位
        }
        iterator end = _finish;
        while (end != pos)                // 从后往前搬
        {
            *end = *(end - 1);
            --end;
        }
        *pos = x;
        ++_finish;
        return pos;
    }

    iterator erase(iterator pos)
    {
        assert(pos >= _start && pos < _finish);
        iterator it = pos + 1;
        while (it != _finish)
        {
            *(it - 1) = *it;
            ++it;
        }
        --_finish;
        return pos;
    }

    void swap(vector<T>& v)
    {
        std::swap(_start, v._start);
        std::swap(_finish, v._finish);
        std::swap(_endofstorage, v._endofstorage);
    }

private:
    iterator _start;
    iterator _finish;
    iterator _endofstorage;
};
}

// ============================================================================
// 二、bite::string —— 管资源的自定义类型（第七节实验用）
// ============================================================================
namespace bite
{
class string
{
public:
    string(const char* str = "")
    {
        _str = new char[strlen(str) + 1];
        strcpy(_str, str);
    }
    string(const string& s)
        : _str(new char[strlen(s._str) + 1])
    {
        strcpy(_str, s._str);
    }
    string& operator=(const string& s)
    {
        if (this != &s)
        {
            char* tmp = new char[strlen(s._str) + 1];
            strcpy(tmp, s._str);
            delete[] _str;
            _str = tmp;
        }
        return *this;
    }
    ~string()
    {
        delete[] _str;
        _str = nullptr;
    }
    const char* c_str() const { return _str; }
private:
    char* _str;
};
}

// ============================================================================
// 三、bitec::vector —— memcpy 错误版 reserve（第七节实验 A / C 专用）
// ============================================================================
namespace bitec
{
template <class T>
class vector
{
public:
    typedef T* iterator;

    vector()
        : _start(nullptr), _finish(nullptr), _endofstorage(nullptr)
    {}

    ~vector()
    {
        delete[] _start;
        _start = _finish = _endofstorage = nullptr;
    }

    size_t size() const     { return _finish - _start; }
    size_t capacity() const { return _endofstorage - _start; }

    T& operator[](size_t i) { return _start[i]; }

    iterator begin() { return _start; }
    iterator end()   { return _finish; }

    void reserve(size_t n)
    {
        if (n > capacity())
        {
            size_t sz = size();
            T* tmp = new T[n];
            if (_start)
                memcpy(tmp, _start, sizeof(T) * sz);  // 陷阱：二进制浅拷贝
            delete[] _start;
            _start = tmp;
            _finish = _start + sz;
            _endofstorage = _start + n;
        }
    }

    void push_back(const T& x)
    {
        if (_finish == _endofstorage)
        {
            size_t newcap = capacity() == 0 ? 4 : capacity() * 2;
            reserve(newcap);
        }
        *_finish = x;
        ++_finish;
    }

private:
    iterator _start;
    iterator _finish;
    iterator _endofstorage;
};
}

// ============================================================================
// 四、第一~六节 + 第七节实验 B：手写 vector 全部测试
// ============================================================================
void test_sim()
{
    // 1. 尾插与扩容
    bit::vector<int> v;
    v.push_back(1); v.push_back(2); v.push_back(3); v.push_back(4);
    cout << "[basic] size=" << v.size()
         << " capacity=" << v.capacity() << " data=";
    for (auto e : v) cout << e << " ";
    cout << endl;

    // 2. insert / erase
    v.insert(v.begin() + 1, 99);
    v.erase(v.begin());
    cout << "[insert/erase] data=";
    for (auto e : v) cout << e << " ";
    cout << "| size=" << v.size()
         << " capacity=" << v.capacity() << endl;

    // 3. 空 vector 头插（边界用例）
    bit::vector<int> e;
    e.insert(e.begin(), 7);
    cout << "[empty insert] data=";
    for (auto x : e) cout << x << " ";
    cout << "| size=" << e.size()
         << " capacity=" << e.capacity() << endl;
    e.erase(e.begin());
    cout << "[empty erase] size=" << e.size() << endl;

    // 4. 拷贝构造与赋值
    bit::vector<int> v2(v);
    cout << "[copy ctor] data=";
    for (auto x : v2) cout << x << " ";
    cout << "| capacity=" << v2.capacity() << endl;

    bit::vector<int> v3;
    v3 = v;
    cout << "[assign] data=";
    for (auto x : v3) cout << x << " ";
    cout << "| capacity=" << v3.capacity() << endl;

    // 5. vector 装自定义类型（正确版 reserve 全程安全）
    bit::vector<bite::string> vs;
    vs.push_back("1111");
    vs.push_back("2222");
    vs.push_back("3333");
    vs.push_back("4444");
    vs.push_back("5555");
    cout << "[string vector] data=";
    for (size_t i = 0; i < vs.size(); ++i)
        cout << vs[i].c_str() << " ";
    cout << "| size=" << vs.size()
         << " capacity=" << vs.capacity() << endl;

    // 6. 杨辉三角
    int numRows = 5;
    bit::vector<bit::vector<int>> vv(numRows);
    for (int i = 0; i < numRows; ++i)
        vv[i].resize(i + 1, 1);
    for (int i = 2; i < numRows; ++i)
        for (int j = 1; j < i; ++j)
            vv[i][j] = vv[i - 1][j] + vv[i - 1][j - 1];
    cout << "[yanghui]" << endl;
    for (int i = 0; i < numRows; ++i)
    {
        for (size_t j = 0; j < vv[i].size(); ++j)
            cout << vv[i][j] << " ";
        cout << endl;
    }

    cout << "ALL TESTS PASSED" << endl;
}

// ============================================================================
// 五、第七节实验 C：memcpy + int（对照组，表面正常）
// ============================================================================
void test_memcpy_int()
{
    bitec::vector<int> v;
    for (int i = 1; i <= 5; ++i)
        v.push_back(i * 10);

    cout << "[memcpy + int] data=";
    for (auto e : v) cout << e << " ";
    cout << "| size=" << v.size() << " capacity=" << v.capacity() << endl;
    cout << "still running: main is about to return normally" << endl;
}

// ============================================================================
// 六、第七节实验 A：memcpy + 自定义类型（离开作用域后堆损坏，仅在 -DRUN_CRASH 下调用）
// ============================================================================
void test_memcpy_crash()
{
    {
        bitec::vector<bite::string> v;
        v.push_back("1111"); cout << "push 1 ok" << endl;
        v.push_back("2222"); cout << "push 2 ok" << endl;
        v.push_back("3333"); cout << "push 3 ok" << endl;
        v.push_back("4444"); cout << "push 4 ok" << endl;
        v.push_back("5555"); cout << "push 5 ok" << endl;
        cout << "leaving scope: destructors are about to run..." << endl;
    }
    cout << "end of main" << endl;
}

// ============================================================================
// 七、第十一节：扩容曲线对比
// ============================================================================
static size_t next_simple(size_t cap, size_t /*sz*/)
{
    return cap == 0 ? 4 : cap * 2;
}

static size_t next_vs_like(size_t cap, size_t sz)
{
    size_t geo = cap + cap / 2;
    return max(geo, sz);
}

static size_t next_15_start4(size_t cap, size_t sz)
{
    (void)sz;
    return cap == 0 ? 4 : cap + cap / 2;
}

static void show_growth(const char* name, size_t (*f)(size_t, size_t))
{
    size_t cap = 0;
    cout << name << ": ";
    for (size_t i = 1; i <= 120; ++i)
    {
        if (i > cap)
        {
            cap = f(cap, i);
            cout << cap << " ";
        }
    }
    cout << endl;
}

void test_growth()
{
    show_growth("simple 2x         ", next_simple);
    show_growth("1.5x min(size+1)  ", next_vs_like);
    show_growth("1.5x start-at-4   ", next_15_start4);
}

// ============================================================================
// main：按文章节次顺序跑全部实验
// ============================================================================
int main()
{
    cout << "========== 第一~六节 + 第七节实验 B：手写 vector 全部测试 ==========" << endl;
    test_sim();

    cout << endl << "========== 第七节实验 C：memcpy + int（对照组，表面正常） ==========" << endl;
    test_memcpy_int();

    cout << endl << "========== 第七节实验 A：memcpy + 自定义类型 ==========" << endl;
#ifdef RUN_CRASH
    cout << "RUN_CRASH 已开启：即将复现堆损坏（Windows 退出码 0xC0000374）..." << endl;
    test_memcpy_crash();
#else
    cout << "（该实验会崩溃，默认跳过；加 -DRUN_CRASH 编译可复现）" << endl;
    cout << "复现命令：g++ -std=c++11 -O2 -DRUN_CRASH vector_sim.cpp -o vector_crash" << endl;
#endif

    cout << endl << "========== 第十一节：扩容曲线对比 ==========" << endl;
    test_growth();

    return 0;
}

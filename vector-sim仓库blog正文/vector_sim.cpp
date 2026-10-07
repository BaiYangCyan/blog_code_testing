// ================= vector 模拟实现（教学完整版） =================
// 编译：g++ -std=c++11 -Wall -Wextra vector_sim.cpp -o vector_sim
#include <iostream>
#include <cstring>
#include <cassert>
#include <algorithm>
using namespace std;

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

// ================= 管资源的 string（第七节实验用） =================
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

int main()
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

    // 5. vector 装自定义类型（正确版 reserve 应全程安全）
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
    return 0;
}

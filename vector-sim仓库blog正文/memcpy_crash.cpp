// 实验 A：memcpy 版 reserve + 自定义类型 → 崩溃
// 编译：g++ -std=c++11 memcpy_crash.cpp -o memcpy_crash
// 预期：push 1~5 全部打印成功，离开作用域后在析构阶段崩溃
//       （Windows 退出码 0xC0000374 STATUS_HEAP_CORRUPTION）
// 注意：这是"错误写法演示"，崩溃是实验目的，不是代码 bug
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

namespace bit
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
    {
        bit::vector<bite::string> v;
        v.push_back("1111"); cout << "push 1 ok" << endl;
        v.push_back("2222"); cout << "push 2 ok" << endl;
        v.push_back("3333"); cout << "push 3 ok" << endl;
        v.push_back("4444"); cout << "push 4 ok" << endl;
        v.push_back("5555"); cout << "push 5 ok" << endl;
        cout << "leaving scope: destructors are about to run..." << endl;
    }
    cout << "end of main" << endl;
    return 0;
}

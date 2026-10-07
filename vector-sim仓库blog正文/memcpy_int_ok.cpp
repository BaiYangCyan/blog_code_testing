// 实验 C：memcpy 版 reserve + int → 表面正常（对照组）
// 编译：g++ -std=c++11 memcpy_int_ok.cpp -o memcpy_int_ok
// 预期：正常输出并退出（危险示例：内置类型掩盖了 UB）
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
                memcpy(tmp, _start, sizeof(T) * sz);  // 同样的 memcpy 写法
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

int main()
{
    bit::vector<int> v;
    for (int i = 1; i <= 5; ++i)
        v.push_back(i * 10);

    cout << "[memcpy + int] data=";
    for (auto e : v) cout << e << " ";
    cout << "| size=" << v.size() << " capacity=" << v.capacity() << endl;
    cout << "still running: main is about to return normally" << endl;
    return 0;
}

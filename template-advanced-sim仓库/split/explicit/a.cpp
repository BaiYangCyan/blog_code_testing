#include "a.h"

template <class T>
T Add(const T& left, const T& right)
{
    return left + right;
}

// 显式实例化：强制生成 int / double 两个具体版本
template int Add<int>(const int&, const int&);
template double Add<double>(const double&, const double&);

int Func(int x)
{
    return x + 10;
}

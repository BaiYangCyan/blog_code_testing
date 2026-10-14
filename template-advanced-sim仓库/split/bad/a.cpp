#include "a.h"

// 模板定义写在 .cpp 里：本文件看不到 Add<int> / Add<double> 的调用，
// "按需实例化"不会发生 → 不生成任何具体函数
template <class T>
T Add(const T& left, const T& right)
{
    return left + right;
}

int Func(int x)
{
    return x + 10;
}

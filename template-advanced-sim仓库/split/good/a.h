#pragma once
// 修复方案一（推荐）：声明和定义都放进头文件
template <class T>
T Add(const T& left, const T& right)
{
    return left + right;
}

int Func(int x);

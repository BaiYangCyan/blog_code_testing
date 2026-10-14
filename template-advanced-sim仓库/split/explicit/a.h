#pragma once
// 修复方案二：定义留在 .cpp，但需要"显式实例化"（工程上不推荐）
template <class T>
T Add(const T& left, const T& right);

int Func(int x);

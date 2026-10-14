#pragma once
// 坏例：模板只有声明，定义藏在 a.cpp 里
template <class T>
T Add(const T& left, const T& right);

int Func(int x);

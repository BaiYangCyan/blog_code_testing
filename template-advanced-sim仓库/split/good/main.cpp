#include <iostream>
#include "a.h"
using namespace std;

int main()
{
    cout << Add(1, 2) << endl;       // Add<int>
    cout << Add(1.0, 2.0) << endl;   // Add<double>
    cout << Func(1) << endl;         // 普通函数
    return 0;
}

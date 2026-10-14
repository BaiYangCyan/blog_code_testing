#include <iostream>
#include "a.h"
using namespace std;

int main()
{
    cout << Add(1, 2) << endl;       // 需要 Add<int>
    cout << Add(1.0, 2.0) << endl;   // 需要 Add<double>
    cout << Func(1) << endl;         // 普通函数：不受影响
    return 0;
}

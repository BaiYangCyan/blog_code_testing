#include <iostream>
#include "a.h"
using namespace std;

int main()
{
    cout << Add(1, 2) << endl;       // Add<int>（已显式实例化）
    cout << Add(1.0, 2.0) << endl;   // Add<double>（已显式实例化）
    cout << Func(1) << endl;
    return 0;
}

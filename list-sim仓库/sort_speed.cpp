// 效率对比：list.sort() vs 拷贝到 vector 排序再拷贝回来
// 编译（务必开优化）：g++ -std=c++11 -O2 sort_speed.cpp -o sort_speed
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
    const int N = 1000000;
    srand(1);

    // 方式一：list 自带的 sort（链表归并排序）
    list<int> lt;
    for (int i = 0; i < N; ++i)
        lt.push_back(rand());

    clock_t b1 = clock();
    lt.sort();
    clock_t e1 = clock();

    // 方式二：拷贝到 vector，sort 后再拷贝回来
    srand(1);
    list<int> lt2;
    for (int i = 0; i < N; ++i)
        lt2.push_back(rand());

    clock_t b2 = clock();
    vector<int> v(lt2.begin(), lt2.end());
    sort(v.begin(), v.end());
    lt2.assign(v.begin(), v.end());
    clock_t e2 = clock();

    cout << "N = " << N << "（Release/-O2 环境实测）" << endl;
    cout << "list.sort()                  : " << (double)(e1 - b1) / CLOCKS_PER_SEC << " s" << endl;
    cout << "copy -> vector sort -> back  : " << (double)(e2 - b2) / CLOCKS_PER_SEC << " s" << endl;
    return 0;
}

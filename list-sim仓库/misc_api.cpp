// list 独门接口实测：splice / merge / unique / remove / reverse / swap / std::find
// 编译：g++ -std=c++11 -Wall -Wextra misc_api.cpp -o misc_api
#include <iostream>
#include <list>
#include <algorithm>
using namespace std;

template <class T>
void print(const char* tag, const list<T>& lt)
{
    cout << tag;
    for (const auto& e : lt) cout << e << " ";
    cout << "| size=" << lt.size() << endl;
}

int main()
{
    // ---- splice：剪切，不是粘贴 ----
    cout << "=== splice 整链剪切 ===" << endl;
    {
        list<int> l1{1, 2, 3, 4};
        list<int> l2{10, 20, 30};
        auto it = ++l1.begin();          // 指向 2
        l1.splice(it, l2);               // 把 l2 整条剪到 2 前面
        print("l1: ", l1);
        print("l2: ", l2);
        cout << "it 仍指向 2，解引用 = " << *it << endl;
    }

    cout << endl << "=== splice 自剪切（把第一个结点剪到末尾） ===" << endl;
    {
        list<int> l1{1, 2, 3, 4};
        l1.splice(l1.end(), l1, l1.begin());
        print("l1: ", l1);
    }

    // ---- merge：归并两条有序链表，被合并方变空 ----
    cout << endl << "=== merge ===" << endl;
    {
        list<int> a{1, 3, 5};
        list<int> b{2, 4, 6};
        a.merge(b);
        print("a: ", a);
        print("b: ", b);
    }

    // ---- unique：只删“相邻”的重复元素 ----
    cout << endl << "=== unique ===" << endl;
    {
        list<int> l{1, 1, 2, 2, 3, 1, 1};
        l.unique();
        print("直接 unique: ", l);       // 末尾的 1 还在
        l.sort();
        l.unique();
        print("先 sort 再 unique: ", l);
    }

    // ---- remove：按值删除（erase 是按位置删除） ----
    cout << endl << "=== remove ===" << endl;
    {
        list<int> l{1, 2, 3, 2, 4};
        l.remove(2);
        print("remove(2): ", l);
    }

    // ---- reverse：原地反转 ----
    cout << endl << "=== reverse ===" << endl;
    {
        list<int> l{1, 2, 3, 4, 5};
        l.reverse();
        print("reverse: ", l);
    }

    // ---- swap 与 std::find ----
    cout << endl << "=== swap 与 std::find ===" << endl;
    {
        list<int> a{1, 2, 3};
        list<int> b{7, 8};
        a.swap(b);                       // 只换哨兵指针，O(1)
        print("a: ", a);
        print("b: ", b);
        auto f = std::find(a.begin(), a.end(), 8);   // list 没有成员 find
        if (f != a.end())
            cout << "std::find 找到 8" << endl;
    }

    return 0;
}

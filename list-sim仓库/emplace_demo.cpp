// emplace_back vs push_back：谁少一次拷贝/移动？
// 编译：g++ -std=c++11 -Wall -Wextra emplace_demo.cpp -o emplace_demo
#include <iostream>
#include <list>
using namespace std;

struct Track
{
    static int ctor, copy, move;   // 构造 / 拷贝 / 移动 计数

    int v;

    Track(int v = 0) : v(v) { ++ctor; }
    Track(const Track& o) : v(o.v) { ++copy; }
    Track(Track&& o) noexcept : v(o.v) { ++move; }
};

int Track::ctor = 0;
int Track::copy = 0;
int Track::move = 0;

void reset(const char* tag)
{
    Track::ctor = Track::copy = Track::move = 0;
    cout << tag;
}

void report()
{
    cout << "构造=" << Track::ctor << "，拷贝=" << Track::copy << "，移动=" << Track::move << endl;
}

int main()
{
    list<Track> lt;

    Track named(1);

    reset("[push_back 左值] ");
    lt.push_back(named);               // 拷贝一个已有对象
    report();

    reset("[push_back 右值] ");
    lt.push_back(Track(2));            // 先构造临时对象，再移动进链表
    report();

    reset("[emplace_back  ] ");
    lt.emplace_back(3);                // 参数直接递给结点构造函数，原地构造
    report();

    cout << "list 中现有元素: ";
    for (const auto& e : lt) cout << e.v << " ";
    cout << endl;
    return 0;
}

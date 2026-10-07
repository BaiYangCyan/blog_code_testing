#include <iostream>
#include <algorithm>
using namespace std;

static size_t next_simple(size_t cap, size_t /*sz*/)
{
    return cap == 0 ? 4 : cap * 2;
}

static size_t next_vs_like(size_t cap, size_t sz)
{
    size_t geo = cap + cap / 2;
    return max(geo, sz);
}

static size_t next_15_start4(size_t cap, size_t sz)
{
    (void)sz;
    return cap == 0 ? 4 : cap + cap / 2;
}

static void show(const char* name, size_t (*f)(size_t, size_t))
{
    size_t cap = 0;
    cout << name << ": ";
    for (size_t i = 1; i <= 120; ++i)
    {
        if (i > cap)
        {
            cap = f(cap, i);
            cout << cap << " ";
        }
    }
    cout << endl;
}

int main()
{
    show("simple 2x         ", next_simple);
    show("1.5x min(size+1)  ", next_vs_like);
    show("1.5x start-at-4   ", next_15_start4);
    return 0;
}

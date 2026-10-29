// ============================================================================
// unordered_map / unordered_set 使用与性能实验（单文件版）
// 配套文章：《C++进阶篇（七）：unordered_map 和 unordered_set——哈希桶、负载因子与性能对比》
//
// 说明：本篇只写“使用 + 性能”，不实现哈希表；手写哈希表在下一篇。
// 编译：g++ -std=c++11 -O2 -Wall -Wextra unordered_sim.cpp -o unordered_sim
// ============================================================================
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstddef>
using namespace std;

using Clock = chrono::high_resolution_clock;

static double MsSince(Clock::time_point start)
{
    return chrono::duration<double, milli>(Clock::now() - start).count();
}

// ============================================================================
// 一、unordered_set 基本操作
// ============================================================================
static void test_unordered_set()
{
    cout << "--- unordered_set：去重、查找、删除 ---" << endl;
    unordered_set<int> us = { 3, 1, 3, 5, 2 };

    cout << "Size=" << us.size()
         << "，count(3)=" << us.count(3)
         << "，count(9)=" << us.count(9) << endl;

    vector<int> sorted(us.begin(), us.end());
    sort(sorted.begin(), sorted.end());
    cout << "排序后: ";
    for (int e : sorted) cout << e << " ";
    cout << "（unordered 遍历顺序不保证）" << endl;

    auto it = us.find(3);
    cout << "find(3)=" << (it != us.end()) << "，find(9)=" << (us.find(9) != us.end()) << endl;
    us.erase(3);
    cout << "erase(3) 后 Size=" << us.size() << "，count(3)=" << us.count(3) << endl;
    cout << endl;
}

// ============================================================================
// 二、unordered_map 与词频
// ============================================================================
static void test_unordered_map()
{
    cout << "--- unordered_map：insert / operator[] / 词频 ---" << endl;
    unordered_map<string, string> dict;
    dict.insert({ "sort", "排序" });
    dict.insert({ "left", "左边" });
    dict.insert({ "right", "右边" });

    auto ret = dict.insert({ "left", "不该覆盖" });
    cout << "重复插入 left: second=" << ret.second
         << "，值仍是 " << dict["left"] << endl;

    dict["left"] = "左边, 剩余";
    dict["insert"] = "插入";
    dict["string"];   // 不存在则插入空串

    cout << "left=" << dict["left"]
         << "，insert=" << dict["insert"]
         << "，string=\"" << dict["string"] << "\"" << endl;

    // unordered_map 遍历顺序不保证，这里按 key 排序后再打印，保证输出稳定
    vector<pair<string, string>> kv(dict.begin(), dict.end());
    sort(kv.begin(), kv.end(),
         [](const pair<string, string>& a, const pair<string, string>& b) {
             return a.first < b.first;
         });
    cout << "按 key 排序后: ";
    for (const auto& e : kv) cout << e.first << ":" << e.second << " ";
    cout << endl;

    string arr[] = { "苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果",
                     "西瓜", "苹果", "香蕉", "苹果", "香蕉" };
    unordered_map<string, int> countMap;
    for (const auto& w : arr) countMap[w]++;

    cout << "词频: 苹果=" << countMap["苹果"]
         << "，西瓜=" << countMap["西瓜"]
         << "，香蕉=" << countMap["香蕉"]
         << "，Size=" << countMap.size() << endl;
    cout << endl;
}

// ============================================================================
// 三、bucket / 负载因子 / rehash / reserve
// ============================================================================
static void test_buckets()
{
    cout << "--- bucket、负载因子、rehash ---" << endl;
    unordered_set<int> us;
    us.max_load_factor(0.7f);
    for (int i = 0; i < 100; ++i) us.insert(i);

    cout << fixed << setprecision(3);
    cout << "插入 100 个：bucket_count=" << us.bucket_count()
         << "，load_factor=" << us.load_factor()
         << "，max_load_factor=" << us.max_load_factor() << endl;

    size_t b = us.bucket(42);
    cout << "bucket(42)=" << b
         << "，bucket_size(" << b << ")=" << us.bucket_size(b) << endl;

    us.reserve(500);
    cout << "reserve(500)：bucket_count=" << us.bucket_count()
         << "，load_factor=" << us.load_factor() << endl;

    us.rehash(1000);
    cout << "rehash(1000)：bucket_count=" << us.bucket_count()
         << "，load_factor=" << us.load_factor() << endl;
    cout << "（bucket_count 的具体数值由标准库实现决定）" << endl << endl;
}

// ============================================================================
// 四、unordered_multiset / unordered_multimap
// ============================================================================
static void test_multi()
{
    cout << "--- unordered_multiset / unordered_multimap ---" << endl;
    unordered_multiset<int> ms = { 1, 1, 2, 3, 3, 3 };
    cout << "multiset: count(1)=" << ms.count(1)
         << "，count(3)=" << ms.count(3) << endl;

    auto range = ms.equal_range(3);
    int n = 0;
    for (auto it = range.first; it != range.second; ++it) ++n;
    cout << "equal_range(3) 枚举到 " << n << " 个" << endl;

    unordered_multimap<string, int> mm;
    mm.insert({ "apple", 1 });
    mm.insert({ "apple", 2 });
    mm.insert({ "banana", 3 });
    cout << "multimap: count(apple)=" << mm.count("apple") << "，值: ";
    auto r2 = mm.equal_range("apple");
    for (auto it = r2.first; it != r2.second; ++it)
        cout << it->second << " ";
    cout << endl << endl;
}

// ============================================================================
// 五、set vs unordered_set 性能
// ============================================================================
static void test_performance()
{
    const int N = 1000000;
    cout << "--- set vs unordered_set，N=" << N << " ---" << endl;

    vector<int> v(N);
    for (int i = 0; i < N; ++i) v[i] = i + 1;
    mt19937 rng(2026);
    shuffle(v.begin(), v.end(), rng);

    set<int> s;
    auto t0 = Clock::now();
    for (int e : v) s.insert(e);
    double sInsert = MsSince(t0);

    unordered_set<int> us;
    us.reserve(N);
    t0 = Clock::now();
    for (int e : v) us.insert(e);
    double usInsert = MsSince(t0);

    int hitS = 0;
    t0 = Clock::now();
    for (int e : v) if (s.find(e) != s.end()) ++hitS;
    double sFind = MsSince(t0);

    int hitUs = 0;
    t0 = Clock::now();
    for (int e : v) if (us.find(e) != us.end()) ++hitUs;
    double usFind = MsSince(t0);

    t0 = Clock::now();
    for (int e : v) s.erase(e);
    double sErase = MsSince(t0);

    t0 = Clock::now();
    for (int e : v) us.erase(e);
    double usErase = MsSince(t0);

    cout << fixed << setprecision(2);
    cout << "插入：set " << sInsert << " ms，unordered_set " << usInsert << " ms" << endl;
    cout << "查找：set " << sFind << " ms（命中 " << hitS << "），unordered_set "
         << usFind << " ms（命中 " << hitUs << "）" << endl;
    cout << "删除：set " << sErase << " ms，unordered_set " << usErase << " ms" << endl;
    cout << "剩余：set " << s.size() << "，unordered_set " << us.size() << endl;
    cout << "（时间随机器波动；两边数据、查找/删除顺序完全相同）" << endl << endl;
}

// ============================================================================
// 六、rehash 与迭代器/引用稳定性
// ============================================================================
static void test_rehash_iterator()
{
    cout << "--- rehash：迭代器失效，但引用/指针仍有效 ---" << endl;
    unordered_set<int> us;
    for (int i = 0; i < 100; ++i) us.insert(i);

    auto it = us.begin();
    const int* p = &*it;
    int value = *p;
    size_t before = us.bucket_count();

    us.rehash(before * 4 + 1);   // 原 it 失效，后面不再使用 it
    cout << "rehash 前 bucket_count=" << before
         << "，rehash 后=" << us.bucket_count() << endl;
    cout << "原指针仍可读：" << *p << "（值=" << value << "）" << endl;
    cout << "（原迭代器已失效，不要再用；引用/指针不因 rehash 失效）" << endl;
}

int main()
{
    cout << "========== 一、unordered_set 基本操作 ==========" << endl;
    test_unordered_set();

    cout << "========== 二、unordered_map 与词频 ==========" << endl;
    test_unordered_map();

    cout << "========== 三、bucket 与负载因子 ==========" << endl;
    test_buckets();

    cout << "========== 四、multiset / multimap ==========" << endl;
    test_multi();

    cout << "========== 五、set vs unordered_set 性能 ==========" << endl;
    test_performance();

    cout << "========== 六、rehash 与迭代器稳定性 ==========" << endl;
    test_rehash_iterator();

    return 0;
}

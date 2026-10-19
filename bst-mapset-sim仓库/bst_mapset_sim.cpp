// ============================================================================
// 二叉搜索树 + map/set 一站式实验（单文件版）
// 配套文章：《C++进阶篇（三）：二叉搜索树与 map/set——从手写 BST 到会用关联容器》
//
// 顶部导航（在编辑器里 Ctrl+F 搜函数名，直达文章对应节）：
//   第一节  BST 基本操作（插入/查找/中序/递归版）      → test_bst_basic()
//   第二节  BST 删除：四种情况                          → test_bst_erase()
//   第三节  BST 性能：有序退化 vs 随机                  → test_bst_performance()
//   第四节  KV 模型（词频/词典/拷贝）+ 删除纠偏         → test_kv_bst()
//   第五节  set：去重排序/区间删/两种 find              → test_set()
//   第六节  multiset：值冗余                            → test_multiset()
//   第七节  map：pair/insert/operator[]/词频            → test_map()
//   第八节  multimap：一对多                            → test_multimap()
//   第九节  OJ 实战：349 交集 / 142 环形链表 / 692 前K高频 → test_oj()
//
// 编译运行：
//   g++ -std=c++11 -O2 -Wall -Wextra bst_mapset_sim.cpp -o bst_mapset_sim && ./bst_mapset_sim
// Windows 用户直接双击 build.bat（一键编译 + 运行 + 刷新 output.txt）
// ============================================================================
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <algorithm>
#include <ctime>
#include <cstdlib>
using namespace std;

// ============================================================================
// 一、二叉搜索树（K 模型）：结点 + 插入/查找/删除/中序
// ============================================================================
namespace key
{
template <class K>
struct BSTNode
{
    K _key;
    BSTNode<K>* _left;
    BSTNode<K>* _right;

    BSTNode(const K& key)
        : _key(key), _left(nullptr), _right(nullptr)
    {}
};

template <class K>
class BSTree
{
    typedef BSTNode<K> Node;

public:
    BSTree() {}

    ~BSTree() { Destroy(_root); }

    bool Insert(const K& k)
    {
        if (_root == nullptr)
        {
            _root = new Node(k);
            return true;
        }

        Node* parent = nullptr;
        Node* cur = _root;
        while (cur)
        {
            if (cur->_key < k)
            {
                parent = cur;
                cur = cur->_right;
            }
            else if (cur->_key > k)
            {
                parent = cur;
                cur = cur->_left;
            }
            else
            {
                return false;   // 不支持重复 key（set/map 语义）
            }
        }

        cur = new Node(k);
        if (parent->_key < k)
            parent->_right = cur;
        else
            parent->_left = cur;
        return true;
    }

    // 递归版插入：Node*& 直接改"父结点的指针"，不用再记 parent
    bool InsertR(const K& k) { return _InsertR(_root, k); }

    Node* Find(const K& k)
    {
        Node* cur = _root;
        while (cur)
        {
            if (cur->_key < k)
                cur = cur->_right;
            else if (cur->_key > k)
                cur = cur->_left;
            else
                return cur;
        }
        return nullptr;
    }

    Node* FindR(const K& k) { return _FindR(_root, k); }

    bool Erase(const K& k)
    {
        Node* parent = nullptr;
        Node* cur = _root;
        while (cur)
        {
            if (cur->_key < k)
            {
                parent = cur;
                cur = cur->_right;
            }
            else if (cur->_key > k)
            {
                parent = cur;
                cur = cur->_left;
            }
            else
            {
                // 情况 1/2：左为空（叶子也走这里）
                if (cur->_left == nullptr)
                {
                    if (parent == nullptr)
                        _root = cur->_right;
                    else if (parent->_left == cur)
                        parent->_left = cur->_right;
                    else
                        parent->_right = cur->_right;
                    delete cur;
                    return true;
                }
                // 情况 3：右为空
                else if (cur->_right == nullptr)
                {
                    if (parent == nullptr)
                        _root = cur->_left;
                    else if (parent->_left == cur)
                        parent->_left = cur->_left;
                    else
                        parent->_right = cur->_left;
                    delete cur;
                    return true;
                }
                // 情况 4：左右都不空，用右子树最小结点替换
                else
                {
                    Node* rightMinP = cur;       // 关键：必须初始化为 cur
                    Node* rightMin = cur->_right;
                    while (rightMin->_left)
                    {
                        rightMinP = rightMin;
                        rightMin = rightMin->_left;
                    }

                    cur->_key = rightMin->_key;

                    if (rightMinP->_left == rightMin)
                        rightMinP->_left = rightMin->_right;
                    else
                        rightMinP->_right = rightMin->_right;

                    delete rightMin;
                    return true;
                }
            }
        }
        return false;
    }

    void InOrder()
    {
        _InOrder(_root);
        cout << endl;
    }

    // 迭代求高度（避免退化树递归爆栈）
    int Height() const
    {
        if (_root == nullptr) return 0;
        int h = 0;
        queue<pair<Node*, int>> q;
        q.push({ _root, 1 });
        while (!q.empty())
        {
            pair<Node*, int> cur = q.front();
            q.pop();
            if (cur.second > h) h = cur.second;
            if (cur.first->_left) q.push({ cur.first->_left, cur.second + 1 });
            if (cur.first->_right) q.push({ cur.first->_right, cur.second + 1 });
        }
        return h;
    }

private:
    bool _InsertR(Node*& root, const K& k)
    {
        if (root == nullptr)
        {
            root = new Node(k);
            return true;
        }
        if (root->_key > k)
            return _InsertR(root->_left, k);
        if (root->_key < k)
            return _InsertR(root->_right, k);
        return false;
    }

    Node* _FindR(Node* root, const K& k)
    {
        if (root == nullptr) return nullptr;
        if (root->_key == k) return root;
        if (root->_key < k) return _FindR(root->_right, k);
        return _FindR(root->_left, k);
    }

    void _InOrder(Node* root)
    {
        if (root == nullptr) return;
        _InOrder(root->_left);
        cout << root->_key << " ";
        _InOrder(root->_right);
    }

    void Destroy(Node* root)
    {
        // 迭代后序释放：退化树深 N，递归析构会爆栈
        stack<Node*> st;
        if (root) st.push(root);
        while (!st.empty())
        {
            Node* cur = st.top();
            st.pop();
            if (cur->_left) st.push(cur->_left);
            if (cur->_right) st.push(cur->_right);
            delete cur;
        }
    }

    Node* _root = nullptr;
};
}

void test_bst_basic()
{
    key::BSTree<int> t;
    int a[] = { 8, 3, 1, 10, 6, 4, 7, 14, 13 };
    for (auto e : a) t.Insert(e);

    cout << "中序（天然有序）: ";
    t.InOrder();

    cout << "Find(7): " << (t.Find(7) ? "找到" : "没找到")
         << "，Find(99): " << (t.Find(99) ? "找到" : "没找到") << endl;
    cout << "递归版 FindR(13): " << (t.FindR(13) ? "找到" : "没找到") << endl;

    key::BSTree<int> tr;
    int b[] = { 5, 2, 8, 1, 9 };
    for (auto e : b) tr.InsertR(e);
    cout << "递归插入后的中序: ";
    tr.InOrder();
}

// ============================================================================
// 二、删除：四种情况逐个演示
// ============================================================================
void test_bst_erase()
{
    int a[] = { 8, 3, 1, 10, 6, 4, 7, 14, 13 };
    key::BSTree<int> t;
    for (auto e : a) t.Insert(e);

    cout << "原始: "; t.InOrder();

    t.Erase(1);   // 情况 1：叶子
    cout << "删 1（叶子）: "; t.InOrder();

    t.Erase(14);  // 情况 3：右空、左孩子 13 顶上
    cout << "删 14（右空，托孤 13）: "; t.InOrder();

    t.Erase(3);   // 情况 4：两个孩子，右子树最小 4 替换
    cout << "删 3（两孩子，后继 4 替换）: "; t.InOrder();

    t.Erase(8);   // 情况 4：根 + 两个孩子
    cout << "删 8（根，后继 10 替换）: "; t.InOrder();

    t.Erase(100); // 不存在
    cout << "删不存在的 100: 返回 false" << endl;

    for (auto e : a) t.Erase(e);
    cout << "全部删完后: "; t.InOrder();
}

// ============================================================================
// 三、性能：有序插入退化 vs 随机插入
// ============================================================================
void test_bst_performance()
{
    const int N = 10000;

    // 1) 有序插入：退化成单支链
    key::BSTree<int> t1;
    for (int i = 1; i <= N; ++i) t1.Insert(i);

    // 2) 随机插入：用固定种子打乱
    vector<int> v(N);
    for (int i = 0; i < N; ++i) v[i] = i + 1;
    srand(2026);
    for (int i = N - 1; i > 0; --i)
        swap(v[i], v[rand() % (i + 1)]);
    key::BSTree<int> t2;
    for (auto e : v) t2.Insert(e);

    cout << "N = " << N << " 个结点" << endl;
    cout << "有序插入的高度: " << t1.Height() << "（退化成链表）" << endl;
    cout << "随机插入的高度: " << t2.Height() << "（接近平衡）" << endl;

    // 各做 2000 次查找，比较耗时（结果必须被使用，防 -O2 把循环优化掉）
    const int QN = 2000;
    vector<int> queries(QN);
    srand(2026);
    for (int i = 0; i < QN; ++i) queries[i] = rand() % N + 1;

    int found1 = 0;
    clock_t b1 = clock();
    for (int i = 0; i < QN; ++i)
        if (t1.Find(queries[i])) ++found1;
    clock_t e1 = clock();

    int found2 = 0;
    clock_t b2 = clock();
    for (int i = 0; i < QN; ++i)
        if (t2.Find(queries[i])) ++found2;
    clock_t e2 = clock();

    cout << QN << " 次查找（同一组关键字）：有序树命中 " << found1
         << " 个，耗时 " << fixed << setprecision(1)
         << (e1 - b1) * 1000.0 / CLOCKS_PER_SEC << " ms" << endl;
    cout << "                    随机树命中 " << found2
         << " 个，耗时 " << (e2 - b2) * 1000.0 / CLOCKS_PER_SEC << " ms" << endl;
    cout << "（这就是为什么需要 AVL / 红黑树——把最坏 O(N) 压回 O(logN)）" << endl;
}

// ============================================================================
// 四、KV 模型：词频 / 词典 / 深拷贝 + 删除纠偏
// ============================================================================
namespace kv
{
template <class K, class V>
struct BSTNode
{
    K _key;
    V _value;
    BSTNode<K, V>* _left;
    BSTNode<K, V>* _right;

    BSTNode(const K& key, const V& value)
        : _key(key), _value(value), _left(nullptr), _right(nullptr)
    {}
};

template <class K, class V>
class BSTree
{
    typedef BSTNode<K, V> Node;

public:
    BSTree() {}
    BSTree(const BSTree<K, V>& t) { _root = Copy(t._root); }
    BSTree<K, V>& operator=(BSTree<K, V> t)
    {
        swap(_root, t._root);
        return *this;
    }
    ~BSTree() { Destroy(_root); }

    bool Insert(const K& k, const V& v)
    {
        if (_root == nullptr)
        {
            _root = new Node(k, v);
            return true;
        }
        Node* parent = nullptr;
        Node* cur = _root;
        while (cur)
        {
            if (cur->_key < k) { parent = cur; cur = cur->_right; }
            else if (cur->_key > k) { parent = cur; cur = cur->_left; }
            else return false;
        }
        cur = new Node(k, v);
        if (parent->_key < k) parent->_right = cur;
        else parent->_left = cur;
        return true;
    }

    Node* Find(const K& k)
    {
        Node* cur = _root;
        while (cur)
        {
            if (cur->_key < k) cur = cur->_right;
            else if (cur->_key > k) cur = cur->_left;
            else return cur;
        }
        return nullptr;
    }

    // fixed = true：删除两孩子结点时 key 和 value 一起替换（正确）
    // fixed = false：只替换 key（原版写法，会造成 key/value 错位）
    bool Erase(const K& k, bool fixed = true)
    {
        Node* parent = nullptr;
        Node* cur = _root;
        while (cur)
        {
            if (cur->_key < k) { parent = cur; cur = cur->_right; }
            else if (cur->_key > k) { parent = cur; cur = cur->_left; }
            else
            {
                if (cur->_left == nullptr)
                {
                    if (parent == nullptr) _root = cur->_right;
                    else if (parent->_left == cur) parent->_left = cur->_right;
                    else parent->_right = cur->_right;
                    delete cur;
                    return true;
                }
                else if (cur->_right == nullptr)
                {
                    if (parent == nullptr) _root = cur->_left;
                    else if (parent->_left == cur) parent->_left = cur->_left;
                    else parent->_right = cur->_left;
                    delete cur;
                    return true;
                }
                else
                {
                    Node* rightMinP = cur;
                    Node* rightMin = cur->_right;
                    while (rightMin->_left)
                    {
                        rightMinP = rightMin;
                        rightMin = rightMin->_left;
                    }

                    cur->_key = rightMin->_key;
                    if (fixed) cur->_value = rightMin->_value;   // ← 纠偏点

                    if (rightMinP->_left == rightMin)
                        rightMinP->_left = rightMin->_right;
                    else
                        rightMinP->_right = rightMin->_right;

                    delete rightMin;
                    return true;
                }
            }
        }
        return false;
    }

    void InOrder()
    {
        _InOrder(_root);
        cout << endl;
    }

private:
    void _InOrder(Node* root)
    {
        if (root == nullptr) return;
        _InOrder(root->_left);
        cout << root->_key << ":" << root->_value << " ";
        _InOrder(root->_right);
    }
    void Destroy(Node* root)
    {
        if (root == nullptr) return;
        Destroy(root->_left);
        Destroy(root->_right);
        delete root;
    }
    Node* Copy(Node* root)
    {
        if (root == nullptr) return nullptr;
        Node* newRoot = new Node(root->_key, root->_value);
        newRoot->_left = Copy(root->_left);
        newRoot->_right = Copy(root->_right);
        return newRoot;
    }
    Node* _root = nullptr;
};
}

void test_kv_bst()
{
    // 1) 词频统计：key = 水果，value = 次数
    string arr[] = { "苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };
    kv::BSTree<string, int> countTree;
    for (const auto& str : arr)
    {
        auto ret = countTree.Find(str);
        if (ret == nullptr)
            countTree.Insert(str, 1);
        else
            ret->_value++;
    }
    cout << "词频（按 key 升序）: ";
    countTree.InOrder();

    // 2) 词典：静态查询（交互式 cin 版本见仓库说明）
    kv::BSTree<string, string> dict;
    dict.Insert("left", "左边");
    dict.Insert("right", "右边");
    dict.Insert("insert", "插入");
    dict.Insert("string", "字符串");
    const char* words[] = { "left", "right", "hello" };
    for (auto w : words)
    {
        auto ret = dict.Find(w);
        if (ret) cout << "查 " << w << " -> " << ret->_value << endl;
        else cout << "查 " << w << " -> 无此单词" << endl;
    }

    // 3) 拷贝构造 / 赋值（深拷贝）
    kv::BSTree<string, int> copy(countTree);
    copy.Insert("梨", 1);
    cout << "拷贝后副本: "; copy.InOrder();
    cout << "原树不变  : "; countTree.InOrder();

    // 4) 删除纠偏：只复制 key 会 key/value 错位
    kv::BSTree<int, string> bad, good;
    int ks[] = { 2, 1, 3 };                 // 根 2 左右都有孩子，才会走"两孩子"分支
    const char* vs[] = { "b", "a", "c" };
    for (int i = 0; i < 3; ++i) { bad.Insert(ks[i], vs[i]); good.Insert(ks[i], vs[i]); }

    bad.Erase(2, false);    // 只复制 key 的写法
    good.Erase(2, true);    // 修正：key + value 一起复制
    cout << "只换 key 的写法删除根 2 后: "; bad.InOrder();
    cout << "修正写法（key+value）删除根 2 后: "; good.InOrder();
    cout << "（前者变成 3:b，value 还是 b，正确应是 3:c；后者 3:c，正确）" << endl;
}

// ============================================================================
// 五、set：去重 + 排序
// ============================================================================
void test_set()
{
    set<int> s = { 4, 2, 7, 2, 8, 5, 9 };
    cout << "set 遍历（去重+升序）: ";
    for (auto e : s) cout << e << " ";
    cout << endl;

    // insert 返回值：pair<iterator,bool>
    pair<set<int>::iterator, bool> ret = s.insert(7);
    cout << "重复插入 7: second = " << ret.second << "（false 表示已存在）" << endl;

    // erase 三种用法
    s.erase(s.begin());          // 删最小值
    cout << "erase(begin()) 删最小值: ";
    for (auto e : s) cout << e << " ";
    cout << endl;

    s.erase(4);                  // 按值删
    cout << "erase(4): ";
    for (auto e : s) cout << e << " ";
    cout << endl;

    auto pos = s.find(8);        // 成员 find：O(logN)
    if (pos != s.end()) s.erase(pos);
    cout << "find(8)+erase: ";
    for (auto e : s) cout << e << " ";
    cout << endl;

    // 区间删除：删除 [30, 60]
    set<int> r;
    for (int i = 1; i < 10; ++i) r.insert(i * 10);
    cout << "原区间集合: ";
    for (auto e : r) cout << e << " ";
    cout << endl;
    r.erase(r.lower_bound(30), r.upper_bound(60));
    cout << "删 [30,60] 后: ";
    for (auto e : r) cout << e << " ";
    cout << endl;

    // 降序 set：第二个模板参数
    set<int, greater<int>> desc = { 3, 5, 1, 4 };
    cout << "greater 降序: ";
    for (auto e : desc) cout << e << " ";
    cout << endl;

    // string set：按字典序
    set<string> ss = { "sort", "insert", "add" };
    cout << "string set（字典序）: ";
    for (auto& e : ss) cout << e << " ";
    cout << endl;

    // 成员 find O(logN) vs 算法 find O(N)
    auto p1 = find(s.begin(), s.end(), 9);   // 线性
    auto p2 = s.find(9);                     // 红黑树
    cout << "算法 find 命中: " << (p1 != s.end())
         << "，成员 find 命中: " << (p2 != s.end())
         << "（复杂度 O(N) vs O(logN)）" << endl;
}

// ============================================================================
// 六、multiset：值冗余
// ============================================================================
void test_multiset()
{
    multiset<int> ms = { 4, 2, 7, 2, 4, 8, 4, 5, 4, 9 };
    cout << "multiset（排序、不去重）: ";
    for (auto e : ms) cout << e << " ";
    cout << endl;

    cout << "count(4) = " << ms.count(4) << "（实际个数）" << endl;

    auto pos = ms.find(4);   // 常见实现返回中序第一个 4
    cout << "find(4) 后连续 ++ 枚举所有 4: ";
    while (pos != ms.end() && *pos == 4)
    {
        cout << *pos << " ";
        ++pos;
    }
    cout << endl;

    // 只删一个：erase(find(x))；删全部：erase(x)
    ms.erase(ms.find(4));
    cout << "erase(find(4)) 只删一个后 count(4) = " << ms.count(4) << endl;

    ms.erase(4);
    cout << "erase(4) 删全部后 count(4) = " << ms.count(4) << endl;
    cout << "剩余: ";
    for (auto e : ms) cout << e << " ";
    cout << endl;
}

// ============================================================================
// 七、map：键值对
// ============================================================================
void test_map()
{
    map<string, string> dict = { {"left", "左边"}, {"right", "右边"}, {"insert", "插入"} };

    // 4 种插入方式
    pair<string, string> kv1("first", "第一个");
    dict.insert(kv1);
    dict.insert(pair<string, string>("second", "第二个"));
    dict.insert(make_pair("sort", "排序"));
    dict.insert({ "auto", "自动的" });

    // key 已存在：insert 失败，不覆盖
    auto ret = dict.insert({ "left", "左边、剩余" });
    cout << "重复插入 left: second = " << ret.second
         << "，现有值仍是: " << dict["left"] << endl;

    // 遍历：按 key 升序，it->second 可改、it->first 不可改
    cout << "map 遍历（key 升序）:" << endl;
    for (auto& kv : dict)
        cout << "  " << kv.first << ":" << kv.second << endl;

    // 修改 value
    dict["left"] = "左边（已修改）";
    cout << "修改后 left = " << dict["left"] << endl;

    // operator[]：不存在则插入默认值
    dict["brandnew"];
    cout << "dict[\"brandnew\"] 后 size = " << dict.size()
         << "，值 = \"" << dict["brandnew"] << "\"" << endl;

    // 词频统计：find+insert 与 operator[] 两种写法结果一致
    string arr[] = { "苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };

    map<string, int> m1;
    for (const auto& s : arr)
    {
        auto it = m1.find(s);
        if (it == m1.end()) m1.insert({ s, 1 });
        else it->second++;
    }

    map<string, int> m2;
    for (const auto& s : arr) m2[s]++;   // operator[]：插入 + 修改二合一

    bool same = (m1.size() == m2.size());
    for (auto& kv : m1)
        if (m2[kv.first] != kv.second) same = false;
    cout << "两种词频写法结果一致: " << same << "，共 " << m1.size() << " 种水果" << endl;

    for (auto& kv : m1)
        cout << kv.first << ":" << kv.second << " ";
    cout << endl;
}

// ============================================================================
// 八、multimap：一对多
// ============================================================================
void test_multimap()
{
    multimap<string, string> mm;
    mm.insert({ "apple", "red" });
    mm.insert({ "apple", "green" });
    mm.insert({ "banana", "yellow" });

    cout << "multimap count(apple) = " << mm.count("apple") << endl;

    auto range = mm.equal_range("apple");
    cout << "equal_range 枚举 apple: ";
    for (auto it = range.first; it != range.second; ++it)
        cout << it->first << ":" << it->second << " ";
    cout << endl;

    mm.erase("apple");   // 删除所有等价 key
    cout << "erase(apple) 后 size = " << mm.size() << "，剩余: ";
    for (auto& kv : mm) cout << kv.first << ":" << kv.second << " ";
    cout << endl;
    cout << "（multimap 没有 operator[]——key 可重复，不知道该返回哪一个 value）" << endl;
}

// ============================================================================
// 九、OJ 实战
// ============================================================================
namespace oj
{
struct ListNode
{
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

// 349. 两个数组的交集：set 去重排序 + 双指针
vector<int> Intersection(vector<int>& nums1, vector<int>& nums2)
{
    set<int> s1(nums1.begin(), nums1.end());
    set<int> s2(nums2.begin(), nums2.end());
    vector<int> ret;
    auto it1 = s1.begin();
    auto it2 = s2.begin();
    while (it1 != s1.end() && it2 != s2.end())
    {
        if (*it1 < *it2) ++it1;
        else if (*it1 > *it2) ++it2;
        else
        {
            ret.push_back(*it1);
            ++it1;
            ++it2;
        }
    }
    return ret;
}

// 142. 环形链表 II：set 记录"访问过的结点地址"，首次重复即入口
ListNode* DetectCycle(ListNode* head)
{
    set<ListNode*> visited;
    ListNode* cur = head;
    while (cur)
    {
        auto ret = visited.insert(cur);   // insert 返回值：second == false 表示已存在
        if (!ret.second) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// 692. 前 K 个高频单词：map 统计 + sort 双条件比较器
struct Cmp
{
    bool operator()(const pair<string, int>& x, const pair<string, int>& y) const
    {
        return x.second > y.second || (x.second == y.second && x.first < y.first);
    }
};

vector<string> TopKFrequent(vector<string>& words, int k)
{
    map<string, int> countMap;
    for (auto& w : words) countMap[w]++;

    vector<pair<string, int>> v(countMap.begin(), countMap.end());
    sort(v.begin(), v.end(), Cmp());

    vector<string> ret;
    for (int i = 0; i < k && i < (int)v.size(); ++i)
        ret.push_back(v[i].first);
    return ret;
}
}

void test_oj()
{
    // 349 交集
    vector<int> n1 = { 4, 9, 5 }, n2 = { 9, 4, 9, 8, 4 };
    vector<int> inter = oj::Intersection(n1, n2);
    cout << "349 交集: ";
    for (auto e : inter) cout << e << " ";
    cout << endl;

    // 142 环形链表：3 -> 2 -> 0 -> -4 -> 回到 2
    oj::ListNode* a = new oj::ListNode(3);
    oj::ListNode* b = new oj::ListNode(2);
    oj::ListNode* c = new oj::ListNode(0);
    oj::ListNode* d = new oj::ListNode(-4);
    a->next = b; b->next = c; c->next = d; d->next = b;
    oj::ListNode* entry = oj::DetectCycle(a);
    cout << "142 入环点: " << (entry ? entry->val : -1) << "（无环返回 -1）" << endl;

    // 无环对照
    oj::ListNode* x = new oj::ListNode(1);
    oj::ListNode* y = new oj::ListNode(2);
    x->next = y;
    cout << "142 无环链表: " << (oj::DetectCycle(x) ? "有环" : "无环") << endl;

    // 692 前 K 个高频单词
    vector<string> words = { "i", "love", "leetcode", "i", "love", "coding" };
    vector<string> top = oj::TopKFrequent(words, 2);
    cout << "692 前 2 高频: ";
    for (auto& w : top) cout << w << " ";
    cout << endl;

    // 同频率按字典序
    vector<string> words2 = { "the", "day", "is", "sunny", "the", "the", "the", "sunny", "is", "is" };
    vector<string> top2 = oj::TopKFrequent(words2, 4);
    cout << "692 并列频率（字典序）: ";
    for (auto& w : top2) cout << w << " ";
    cout << endl;
}

// ============================================================================
// main：按文章节次顺序跑全部实验
// ============================================================================
int main()
{
    cout << "========== 第一节 BST 基本操作 ==========" << endl;
    test_bst_basic();

    cout << endl << "========== 第二节 BST 删除四种情况 ==========" << endl;
    test_bst_erase();

    cout << endl << "========== 第三节 BST 性能：有序退化 vs 随机 ==========" << endl;
    test_bst_performance();

    cout << endl << "========== 第四节 KV 模型 + 删除纠偏 ==========" << endl;
    test_kv_bst();

    cout << endl << "========== 第五节 set ==========" << endl;
    test_set();

    cout << endl << "========== 第六节 multiset ==========" << endl;
    test_multiset();

    cout << endl << "========== 第七节 map ==========" << endl;
    test_map();

    cout << endl << "========== 第八节 multimap ==========" << endl;
    test_multimap();

    cout << endl << "========== 第九节 OJ 实战 ==========" << endl;
    test_oj();

    return 0;
}

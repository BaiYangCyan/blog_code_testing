// ============================================================================
// AVL 树一站式实验（单文件版）
// 配套文章：《C++进阶篇（四）：AVL 树——从 BST 退化到自平衡，四种旋转全拆解》
//
// 顶部导航（Ctrl+F 搜函数名，直达文章对应节）：
//   一、四种旋转最小用例（LL/RR/LR/RL）        → test_basic_rotations()
//   二、双旋的 BF 三种修正（bf = -1/0/+1）      → test_double_bf_cases()
//   三、两组课堂序列（含双旋场景）              → test_course_sequences()
//   四、10 万结点：AVL vs 普通 BST              → test_ordered_vs_bst()
//   五、10 万随机插入 + 四道自检                → test_random_insert()
//
// 编译运行：
//   g++ -std=c++11 -O2 -Wall -Wextra avl_sim.cpp -o avl_sim && ./avl_sim
// Windows 用户直接双击 build.bat（一键编译 + 运行 + 刷新 output.txt）
// ============================================================================
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <cassert>
#include <cstdlib>
using namespace std;

// ============================================================================
// 一、AVL 树：结点 + 插入 + 四种旋转 + 查找 + 自检
// ============================================================================
namespace avl
{
template <class K, class V>
struct AVLTreeNode
{
    pair<K, V> _kv;
    AVLTreeNode<K, V>* _left;
    AVLTreeNode<K, V>* _right;
    AVLTreeNode<K, V>* _parent;   // 向上更新 BF 用
    int _bf;                      // balance factor = 右高 - 左高

    AVLTreeNode(const pair<K, V>& kv)
        : _kv(kv)
        , _left(nullptr)
        , _right(nullptr)
        , _parent(nullptr)
        , _bf(0)
    {}
};

template <class K, class V>
class AVLTree
{
    typedef AVLTreeNode<K, V> Node;

public:
    AVLTree() : _root(nullptr), _verbose(false), _lastRotateBf(0) {}
    ~AVLTree() { Destroy(_root); }

    // 单文件实验代码，禁止拷贝，避免浅拷贝 double free
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    void SetVerbose(bool v) { _verbose = v; }
    const string& LastRotation() const { return _lastRotation; }
    int LastRotateBf() const { return _lastRotateBf; }

    bool Insert(const pair<K, V>& kv)
    {
        _lastRotation = "无";
        _lastRotateBf = 0;

        if (_root == nullptr)
        {
            _root = new Node(kv);
            if (_verbose) cout << "  树空，直接作为根" << endl;
            return true;
        }

        Node* parent = nullptr;
        Node* cur = _root;
        while (cur)
        {
            if (cur->_kv.first < kv.first)
            {
                parent = cur;
                cur = cur->_right;
            }
            else if (cur->_kv.first > kv.first)
            {
                parent = cur;
                cur = cur->_left;
            }
            else
            {
                return false;   // 不支持重复 key
            }
        }

        cur = new Node(kv);
        cur->_parent = parent;
        bool isLeft = parent->_kv.first > kv.first;
        if (isLeft)
            parent->_left = cur;
        else
            parent->_right = cur;
        if (_verbose)
            cout << "  挂到 " << parent->_kv.first << (isLeft ? " 的左边" : " 的右边") << endl;

        // 从插入点向根更新 BF
        while (parent)
        {
            int oldBf = parent->_bf;
            if (cur == parent->_left)
                --parent->_bf;
            else
                ++parent->_bf;

            if (_verbose)
                cout << "  更新 " << parent->_kv.first << " 的 bf: "
                     << oldBf << " -> " << parent->_bf << endl;

            if (parent->_bf == 0)
            {
                // 插入在矮的那边，子树高度没变，停止
                break;
            }
            else if (parent->_bf == 1 || parent->_bf == -1)
            {
                // 子树高度 +1，继续往上
                cur = parent;
                parent = parent->_parent;
            }
            else if (parent->_bf == 2 || parent->_bf == -2)
            {
                string rot;
                if (parent->_bf == -2 && cur->_bf == -1)
                    rot = "右单旋";
                else if (parent->_bf == 2 && cur->_bf == 1)
                    rot = "左单旋";
                else if (parent->_bf == -2 && cur->_bf == 1)
                    rot = "左右双旋";
                else if (parent->_bf == 2 && cur->_bf == -1)
                    rot = "右左双旋";
                else
                {
                    assert(false);
                }

                _lastRotation = rot;
                if (_verbose)
                    cout << "  失衡点 " << parent->_kv.first << " bf=" << parent->_bf
                         << "，孩子 " << cur->_kv.first << " bf=" << cur->_bf
                         << " -> " << rot << endl;

                if (rot == "右单旋") RotateR(parent);
                else if (rot == "左单旋") RotateL(parent);
                else if (rot == "左右双旋") RotateLR(parent);
                else RotateRL(parent);

                if (_verbose)
                    cout << "  旋转后: " << DumpInline() << endl;
                break;
            }
            else
            {
                assert(false);
            }
        }

        return true;
    }

    Node* Find(const K& key)
    {
        Node* cur = _root;
        while (cur)
        {
            if (cur->_kv.first < key)
                cur = cur->_right;
            else if (cur->_kv.first > key)
                cur = cur->_left;
            else
                return cur;
        }
        return nullptr;
    }

    void InOrder()
    {
        _InOrder(_root);
        cout << endl;
    }

    int Height() const { return _Height(_root); }
    int Size() const { return _Size(_root); }

    // 四道自检：高度差、BF 一致性、parent 指针、BST 中序有序
    bool IsBalanceTree() const
    {
        if (_root && _root->_parent != nullptr) return false;
        Node* prev = nullptr;
        return _IsBalance(_root) && _CheckParent(_root, nullptr) && _CheckOrder(_root, prev);
    }

    // 紧凑树形：key(bf)[左, 右]
    string DumpInline() const { return _DumpInline(_root); }

    // 旋转轴心打印（右子树在上，便于看层级）
    void Dump() const { _Dump(_root, 0); }

private:
    void RotateR(Node* parent)
    {
        Node* subL = parent->_left;
        Node* subLR = subL->_right;

        // 1) subLR 过继给 parent 的左边
        parent->_left = subLR;
        if (subLR) subLR->_parent = parent;

        // 2) parent 挂到 subL 的右边
        Node* parentParent = parent->_parent;
        subL->_right = parent;
        parent->_parent = subL;

        // 3) subL 接到上一层（或成为新根）
        if (parentParent == nullptr)
        {
            _root = subL;
            subL->_parent = nullptr;
        }
        else
        {
            if (parent == parentParent->_left)
                parentParent->_left = subL;
            else
                parentParent->_right = subL;
            subL->_parent = parentParent;
        }

        // 单旋后两个结点都平衡
        parent->_bf = 0;
        subL->_bf = 0;
    }

    void RotateL(Node* parent)
    {
        Node* subR = parent->_right;
        Node* subRL = subR->_left;

        parent->_right = subRL;
        if (subRL) subRL->_parent = parent;

        Node* parentParent = parent->_parent;
        subR->_left = parent;
        parent->_parent = subR;

        if (parentParent == nullptr)
        {
            _root = subR;
            subR->_parent = nullptr;
        }
        else
        {
            if (parent == parentParent->_left)
                parentParent->_left = subR;
            else
                parentParent->_right = subR;
            subR->_parent = parentParent;
        }

        parent->_bf = 0;
        subR->_bf = 0;
    }

    void RotateLR(Node* parent)
    {
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        int bf = subLR->_bf;          // 必须先存，旋转会改掉它
        _lastRotateBf = bf;

        RotateL(parent->_left);
        RotateR(parent);

        if (bf == 0)
        {
            subL->_bf = 0;
            subLR->_bf = 0;
            parent->_bf = 0;
        }
        else if (bf == -1)
        {
            subL->_bf = 0;
            subLR->_bf = 0;
            parent->_bf = 1;
        }
        else if (bf == 1)
        {
            subL->_bf = -1;
            subLR->_bf = 0;
            parent->_bf = 0;
        }
        else
        {
            assert(false);
        }
    }

    void RotateRL(Node* parent)
    {
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        int bf = subRL->_bf;
        _lastRotateBf = bf;

        RotateR(parent->_right);
        RotateL(parent);

        if (bf == 0)
        {
            subR->_bf = 0;
            subRL->_bf = 0;
            parent->_bf = 0;
        }
        else if (bf == 1)
        {
            subR->_bf = 0;
            subRL->_bf = 0;
            parent->_bf = -1;
        }
        else if (bf == -1)
        {
            subR->_bf = 1;
            subRL->_bf = 0;
            parent->_bf = 0;
        }
        else
        {
            assert(false);
        }
    }

    int _Height(Node* root) const
    {
        if (root == nullptr) return 0;
        int lh = _Height(root->_left);
        int rh = _Height(root->_right);
        return lh > rh ? lh + 1 : rh + 1;
    }

    int _Size(Node* root) const
    {
        if (root == nullptr) return 0;
        return _Size(root->_left) + _Size(root->_right) + 1;
    }

    bool _IsBalance(Node* root) const
    {
        if (root == nullptr) return true;
        int lh = _Height(root->_left);
        int rh = _Height(root->_right);
        int diff = rh - lh;
        if (diff < -1 || diff > 1) return false;
        if (root->_bf != diff) return false;
        return _IsBalance(root->_left) && _IsBalance(root->_right);
    }

    bool _CheckParent(Node* root, Node* parent) const
    {
        if (root == nullptr) return true;
        if (root->_parent != parent) return false;
        return _CheckParent(root->_left, root) && _CheckParent(root->_right, root);
    }

    bool _CheckOrder(Node* root, Node*& prev) const
    {
        if (root == nullptr) return true;
        if (!_CheckOrder(root->_left, prev)) return false;
        if (prev && !(prev->_kv.first < root->_kv.first)) return false;
        prev = root;
        return _CheckOrder(root->_right, prev);
    }

    void _InOrder(Node* root)
    {
        if (root == nullptr) return;
        _InOrder(root->_left);
        cout << root->_kv.first << " ";
        _InOrder(root->_right);
    }

    string _DumpInline(Node* root) const
    {
        if (root == nullptr) return "null";
        ostringstream oss;
        oss << root->_kv.first << "(";
        if (root->_bf > 0) oss << "+";
        oss << root->_bf << ")";
        if (root->_left == nullptr && root->_right == nullptr)
            return oss.str();
        oss << "[" << _DumpInline(root->_left) << ", " << _DumpInline(root->_right) << "]";
        return oss.str();
    }

    void _Dump(Node* root, int depth) const
    {
        if (root == nullptr) return;
        _Dump(root->_right, depth + 1);
        cout << string(depth * 4, ' ') << root->_kv.first << "(";
        if (root->_bf > 0) cout << "+";
        cout << root->_bf << ")" << endl;
        _Dump(root->_left, depth + 1);
    }

    void Destroy(Node* root)
    {
        if (root == nullptr) return;
        Destroy(root->_left);
        Destroy(root->_right);
        delete root;
    }

    Node* _root;
    bool _verbose;
    string _lastRotation;
    int _lastRotateBf;
};
}

// ============================================================================
// 二、普通 BST：只用于对照“有序插入退化成链”
// ============================================================================
namespace bst
{
struct Node
{
    int key;
    Node* left;
    Node* right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BSTree
{
public:
    BSTree() : _root(nullptr) {}
    ~BSTree()
    {
        // 迭代后序释放：链深 10 万，递归析构会爆栈
        vector<Node*> st;
        if (_root) st.push_back(_root);
        while (!st.empty())
        {
            Node* cur = st.back();
            st.pop_back();
            if (cur->left) st.push_back(cur->left);
            if (cur->right) st.push_back(cur->right);
            delete cur;
        }
    }

    BSTree(const BSTree&) = delete;
    BSTree& operator=(const BSTree&) = delete;

    void Insert(int k)
    {
        if (_root == nullptr)
        {
            _root = new Node(k);
            return;
        }
        Node* parent = nullptr;
        Node* cur = _root;
        while (cur)
        {
            parent = cur;
            cur = (k < cur->key) ? cur->left : cur->right;
        }
        if (k < parent->key) parent->left = new Node(k);
        else parent->right = new Node(k);
    }

    // 直接构造 1..N 的单链：等价于有序插入的结果，省掉 O(N^2) 建树时间
    void BuildChain(int n)
    {
        _root = new Node(1);
        Node* cur = _root;
        for (int i = 2; i <= n; ++i)
        {
            cur->right = new Node(i);
            cur = cur->right;
        }
    }

    Node* Find(int k) const
    {
        Node* cur = _root;
        while (cur)
        {
            if (k < cur->key) cur = cur->left;
            else if (k > cur->key) cur = cur->right;
            else return cur;
        }
        return nullptr;
    }

    int Height() const
    {
        if (_root == nullptr) return 0;
        int h = 0;
        vector<pair<Node*, int>> st;
        st.push_back({ _root, 1 });
        while (!st.empty())
        {
            pair<Node*, int> cur = st.back();
            st.pop_back();
            if (cur.second > h) h = cur.second;
            if (cur.first->left) st.push_back({ cur.first->left, cur.second + 1 });
            if (cur.first->right) st.push_back({ cur.first->right, cur.second + 1 });
        }
        return h;
    }

private:
    Node* _root;
};
}

// ============================================================================
// 三、实验一：四种旋转的最小用例
// ============================================================================
static void test_basic_rotations()
{
    struct Case
    {
        const char* name;
        int a[3];
    };
    Case cases[] = {
        { "LL：右单旋（左边高，左孩子的左边更高）", { 30, 20, 10 } },
        { "RR：左单旋（右边高，右孩子的右边更高）", { 10, 20, 30 } },
        { "LR：左右双旋（左高 + 左孩子右高，异号）", { 30, 10, 20 } },
        { "RL：右左双旋（右高 + 右孩子左高，异号）", { 10, 30, 20 } }
    };

    for (const auto& c : cases)
    {
        cout << "--- " << c.name << " ---" << endl;
        avl::AVLTree<int, int> t;
        t.SetVerbose(true);
        for (int i = 0; i < 3; ++i)
        {
            cout << "插入 " << c.a[i] << ":" << endl;
            t.Insert({ c.a[i], c.a[i] });
            cout << "  当前树: " << t.DumpInline() << endl;
        }
        t.SetVerbose(false);
        cout << "中序: ";
        t.InOrder();
        cout << "高度=" << t.Height() << "，Size=" << t.Size()
             << "，平衡=" << t.IsBalanceTree()
             << "，最后触发=" << t.LastRotation() << endl;
        cout << endl;
    }
}

// ============================================================================
// 四、实验二：双旋 BF 的三种修正（subLR/subRL 旧 bf = -1 / 0 / +1）
// ============================================================================
static void run_double_case(const char* name, const vector<int>& seq)
{
    avl::AVLTree<int, int> t;
    for (size_t i = 0; i + 1 < seq.size(); ++i)
        t.Insert({ seq[i], seq[i] });

    cout << "--- " << name << " ---" << endl;
    cout << "插入前: " << t.DumpInline() << endl;
    t.SetVerbose(true);
    t.Insert({ seq.back(), seq.back() });
    t.SetVerbose(false);
    cout << "触发: " << t.LastRotation()
         << "，旧 bf=" << t.LastRotateBf()
         << "，结果: " << t.DumpInline() << endl;
    cout << "平衡=" << t.IsBalanceTree() << endl << endl;
}

static void test_double_bf_cases()
{
    // LR：基础树 + 插入 30 的左边 / 30 自己 / 30 的右边，分别得到 bf = -1 / 0 / +1
    run_double_case("LR 双旋，subLR 旧 bf = -1（插在 e）",
        { 50, 20, 70, 10, 30, 60, 80, 5, 15, 25, 35, 22 });
    run_double_case("LR 双旋，subLR 旧 bf = 0（h=0，30 自己就是新结点）",
        { 10, 5, 8 });
    run_double_case("LR 双旋，subLR 旧 bf = +1（插在 f）",
        { 50, 20, 70, 10, 30, 60, 80, 5, 15, 25, 35, 40 });

    // RL：镜像三种情况
    run_double_case("RL 双旋，subRL 旧 bf = -1（插在 e）",
        { 50, 80, 30, 70, 90, 20, 40, 65, 75, 85, 95, 62 });
    run_double_case("RL 双旋，subRL 旧 bf = 0（h=0，12 自己就是新结点）",
        { 10, 15, 12 });
    run_double_case("RL 双旋，subRL 旧 bf = +1（插在 f）",
        { 50, 80, 30, 70, 90, 20, 40, 65, 75, 85, 95, 78 });
}

// ============================================================================
// 五、实验三：两组课堂序列
// ============================================================================
static void test_course_sequences()
{
    struct Case
    {
        const char* name;
        vector<int> seq;
    };
    Case cases[] = {
        { "常规序列（含多种旋转）", { 16, 3, 7, 11, 9, 26, 18, 14, 15 } },
        { "双旋序列（LR/RL 混合）", { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 } }
    };

    for (const auto& c : cases)
    {
        cout << "--- " << c.name << " ---" << endl;
        avl::AVLTree<int, int> t;
        for (int x : c.seq)
            t.Insert({ x, x });
        cout << "中序: ";
        t.InOrder();
        cout << "高度=" << t.Height() << "，Size=" << t.Size()
             << "，平衡=" << t.IsBalanceTree() << endl;
        cout << "树形: " << t.DumpInline() << endl << endl;
    }
}

// ============================================================================
// 六、实验四：10 万结点，有序插入的 AVL vs 普通 BST
// ============================================================================
static void test_ordered_vs_bst()
{
    const int N = 100000;
    cout << "N = " << N << "，全部按 1..N 有序插入" << endl;

    using Clock = chrono::high_resolution_clock;

    auto t0 = Clock::now();
    avl::AVLTree<int, int> t;
    for (int i = 1; i <= N; ++i) t.Insert({ i, i });
    auto t1 = Clock::now();
    double avlInsertMs = chrono::duration<double, milli>(t1 - t0).count();

    cout << "AVL：高度=" << t.Height() << "，Size=" << t.Size()
         << "，平衡=" << t.IsBalanceTree()
         << "，建树耗时=" << fixed << setprecision(1) << avlInsertMs << " ms" << endl;

    // 普通 BST 有序插入会形成 1->2->...->N 的单链，
    // 直接构造同款链，避免 O(N^2) 的建树等待（结构与逐个插入完全一致）
    bst::BSTree b;
    b.BuildChain(N);
    cout << "BST：高度=" << b.Height()
         << "（有序插入退化成单链，与逐个插入结果一致）" << endl;

    const int Q = 2000;
    vector<int> queries(Q);
    mt19937 rng(2026);
    uniform_int_distribution<int> dist(1, N);
    for (int i = 0; i < Q; ++i) queries[i] = dist(rng);

    int hitAvl = 0;
    t0 = Clock::now();
    for (int q : queries)
        if (t.Find(q)) ++hitAvl;
    t1 = Clock::now();
    double avlFindMs = chrono::duration<double, milli>(t1 - t0).count();

    int hitBst = 0;
    t0 = Clock::now();
    for (int q : queries)
        if (b.Find(q)) ++hitBst;
    t1 = Clock::now();
    double bstFindMs = chrono::duration<double, milli>(t1 - t0).count();

    cout << Q << " 次随机查找：AVL 命中 " << hitAvl << "，耗时 "
         << fixed << setprecision(3) << avlFindMs << " ms" << endl;
    cout << Q << " 次随机查找：BST 命中 " << hitBst << "，耗时 "
         << bstFindMs << " ms" << endl;
    if (avlFindMs > 0.0005)
        cout << "倍数差：约 " << fixed << setprecision(1) << bstFindMs / avlFindMs << " 倍" << endl;
    cout << endl;
}

// ============================================================================
// 七、实验五：10 万随机插入 + 四道自检
// ============================================================================
static void test_random_insert()
{
    const int N = 100000;
    vector<int> v(N);
    for (int i = 0; i < N; ++i) v[i] = i + 1;

    mt19937 rng(2026);
    shuffle(v.begin(), v.end(), rng);

    avl::AVLTree<int, int> t;
    for (int x : v) t.Insert({ x, x });

    cout << "随机插入 " << N << " 个：高度=" << t.Height()
         << "，Size=" << t.Size()
         << "，平衡=" << t.IsBalanceTree() << endl;

    int dup = 0;
    for (int x : v)
        if (!t.Insert({ x, x })) ++dup;
    cout << "再插一遍同样的 " << N << " 个：失败 " << dup
         << " 次，Size 仍为 " << t.Size() << endl;
}

// ============================================================================
// main：按文章节次顺序跑全部实验
// ============================================================================
int main()
{
    cout << "========== 一、四种旋转最小用例 ==========" << endl;
    test_basic_rotations();

    cout << "========== 二、双旋的 BF 三种修正 ==========" << endl;
    test_double_bf_cases();

    cout << "========== 三、两组课堂序列 ==========" << endl;
    test_course_sequences();

    cout << "========== 四、10 万结点：AVL vs BST ==========" << endl;
    test_ordered_vs_bst();

    cout << "========== 五、10 万随机插入 + 四道自检 ==========" << endl;
    test_random_insert();

    return 0;
}

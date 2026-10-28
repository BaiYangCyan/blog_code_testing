// ============================================================================
// 红黑树一站式实验（单文件版）
// 配套文章：《C++进阶篇（五）：红黑树——四条规则、三种插入情况与旋转变色》
//
// 顶部导航（Ctrl+F 搜函数名，直达文章对应节）：
//   一、五个最小用例（叔红/LL/RR/LR/RL）      → test_minimal_cases()
//   二、两组课堂序列                          → test_course_sequences()
//   三、10 万有序：RB vs AVL vs BST            → test_ordered_100k()
//   四、10 万随机 + 重复 key                   → test_random_100k()
//
// 编译运行：
//   g++ -std=c++11 -O2 -Wall -Wextra rbtree_sim.cpp -o rbtree_sim && ./rbtree_sim
// Windows 用户直接双击 build.bat（一键编译 + 运行 + 刷新 output.txt）
// ============================================================================
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <cassert>
#include <cstdlib>
using namespace std;

// ============================================================================
// 一、红黑树：结点 + 插入 + 旋转 + 变色 + 四规则验证
// ============================================================================
namespace rb
{
enum Colour
{
    RED,
    BLACK
};

template <class K, class V>
struct RBTreeNode
{
    pair<K, V> _kv;
    RBTreeNode<K, V>* _left;
    RBTreeNode<K, V>* _right;
    RBTreeNode<K, V>* _parent;
    Colour _col;

    RBTreeNode(const pair<K, V>& kv)
        : _kv(kv)
        , _left(nullptr)
        , _right(nullptr)
        , _parent(nullptr)
        , _col(RED)
    {}
};

template <class K, class V>
class RBTree
{
    typedef RBTreeNode<K, V> Node;

public:
    RBTree() : _root(nullptr), _verbose(false), _rotations(0), _recolors(0) {}
    ~RBTree() { Destroy(_root); }

    RBTree(const RBTree&) = delete;
    RBTree& operator=(const RBTree&) = delete;

    void SetVerbose(bool v) { _verbose = v; }
    int Rotations() const { return _rotations; }
    int Recolors() const { return _recolors; }

    bool Insert(const pair<K, V>& kv)
    {
        if (_root == nullptr)
        {
            _root = new Node(kv);
            _root->_col = BLACK;
            if (_verbose) cout << "  空树：根 " << kv.first << " 为黑色" << endl;
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
        cur->_col = RED;
        cur->_parent = parent;
        bool isLeft = parent->_kv.first > kv.first;
        if (isLeft)
            parent->_left = cur;
        else
            parent->_right = cur;

        if (_verbose)
            cout << "  新结点 " << kv.first << " 为红色，挂到 " << parent->_kv.first
                 << (isLeft ? " 左边" : " 右边") << endl;

        if (parent->_col == BLACK && _verbose)
            cout << "  父亲 " << parent->_kv.first << " 是黑色，插入结束" << endl;

        while (parent && parent->_col == RED)
        {
            Node* grandfather = parent->_parent;

            if (parent == grandfather->_left)
            {
                Node* uncle = grandfather->_right;
                if (uncle && uncle->_col == RED)
                {
                    // 情况1：叔叔为红，只变色，g 当作新的 c 继续向上
                    parent->_col = uncle->_col = BLACK;
                    grandfather->_col = RED;
                    ++_recolors;
                    if (_verbose)
                        cout << "  叔叔 " << uncle->_kv.first << " 为红：p/u 变黑，g="
                             << grandfather->_kv.first << " 变红，继续向上" << endl;
                    cur = grandfather;
                    parent = cur->_parent;
                }
                else
                {
                    // 情况2/3：叔叔为黑/空，旋转 + 变色
                    if (cur == parent->_left)
                    {
                        RotateR(grandfather);
                        parent->_col = BLACK;
                        grandfather->_col = RED;
                        ++_recolors;
                        if (_verbose)
                            cout << "  叔叔黑/空：LL -> RotateR(" << grandfather->_kv.first
                                 << ")，p 黑 g 红，结束" << endl;
                    }
                    else
                    {
                        RotateL(parent);
                        RotateR(grandfather);
                        cur->_col = BLACK;
                        grandfather->_col = RED;
                        ++_recolors;
                        if (_verbose)
                            cout << "  叔叔黑/空：LR -> RotateL(" << parent->_kv.first
                                 << ") + RotateR(" << grandfather->_kv.first
                                 << ")，c 黑 g 红，结束" << endl;
                    }
                    break;
                }
            }
            else
            {
                Node* uncle = grandfather->_left;
                if (uncle && uncle->_col == RED)
                {
                    parent->_col = uncle->_col = BLACK;
                    grandfather->_col = RED;
                    ++_recolors;
                    if (_verbose)
                        cout << "  叔叔 " << uncle->_kv.first << " 为红：p/u 变黑，g="
                             << grandfather->_kv.first << " 变红，继续向上" << endl;
                    cur = grandfather;
                    parent = cur->_parent;
                }
                else
                {
                    if (cur == parent->_right)
                    {
                        RotateL(grandfather);
                        parent->_col = BLACK;
                        grandfather->_col = RED;
                        ++_recolors;
                        if (_verbose)
                            cout << "  叔叔黑/空：RR -> RotateL(" << grandfather->_kv.first
                                 << ")，p 黑 g 红，结束" << endl;
                    }
                    else
                    {
                        RotateR(parent);
                        RotateL(grandfather);
                        cur->_col = BLACK;
                        grandfather->_col = RED;
                        ++_recolors;
                        if (_verbose)
                            cout << "  叔叔黑/空：RL -> RotateR(" << parent->_kv.first
                                 << ") + RotateL(" << grandfather->_kv.first
                                 << ")，c 黑 g 红，结束" << endl;
                    }
                    break;
                }
            }
        }

        if (_root->_col == RED)
        {
            _root->_col = BLACK;
            ++_recolors;
            if (_verbose) cout << "  根 " << _root->_kv.first << " 染黑" << endl;
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

    // 四规则 + parent 指针 + BST 中序有序
    bool IsBalanceTree() const
    {
        if (_root == nullptr) return true;
        if (_root->_col != BLACK) return false;

        Node* prev = nullptr;
        if (!_CheckOrder(_root, prev)) return false;
        if (!_CheckParent(_root, nullptr)) return false;

        int refBlack = 0;
        Node* cur = _root;
        while (cur)
        {
            if (cur->_col == BLACK) ++refBlack;
            cur = cur->_left;
        }
        return _CheckColour(_root, 0, refBlack);
    }

    // key(R)[左, 右]；空树 NIL
    string DumpInline() const { return _DumpInline(_root); }

private:
    void RotateL(Node* parent)
    {
        ++_rotations;
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
    }

    void RotateR(Node* parent)
    {
        ++_rotations;
        Node* subL = parent->_left;
        Node* subLR = subL->_right;

        parent->_left = subLR;
        if (subLR) subLR->_parent = parent;

        Node* parentParent = parent->_parent;
        subL->_right = parent;
        parent->_parent = subL;

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
    }

    bool _CheckColour(Node* root, int blackNum, int refNum) const
    {
        if (root == nullptr)
            return blackNum == refNum;

        if (root->_col == RED && root->_parent && root->_parent->_col == RED)
            return false;

        if (root->_col == BLACK) ++blackNum;
        return _CheckColour(root->_left, blackNum, refNum)
            && _CheckColour(root->_right, blackNum, refNum);
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

    void _InOrder(Node* root)
    {
        if (root == nullptr) return;
        _InOrder(root->_left);
        cout << root->_kv.first << " ";
        _InOrder(root->_right);
    }

    string _DumpInline(Node* root) const
    {
        if (root == nullptr) return "NIL";
        ostringstream oss;
        oss << root->_kv.first << "(" << (root->_col == RED ? "R" : "B") << ")";
        if (root->_left == nullptr && root->_right == nullptr)
            return oss.str();
        oss << "[" << _DumpInline(root->_left) << ", " << _DumpInline(root->_right) << "]";
        return oss.str();
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
    int _rotations;   // 单旋次数，双旋按 2 次计
    int _recolors;    // 变色事件次数
};
}

// ============================================================================
// 二、AVL 对照：只保留插入、查找、旋转计数
// ============================================================================
namespace avl_cmp
{
struct Node
{
    int key;
    int bf;
    Node* left;
    Node* right;
    Node* parent;

    Node(int k) : key(k), bf(0), left(nullptr), right(nullptr), parent(nullptr) {}
};

class AVLTree
{
public:
    AVLTree() : _root(nullptr), _rotations(0) {}
    ~AVLTree() { Destroy(_root); }

    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    int Rotations() const { return _rotations; }
    int Height() const { return _Height(_root); }
    int Size() const { return _Size(_root); }

    bool Insert(int key)
    {
        if (_root == nullptr)
        {
            _root = new Node(key);
            return true;
        }

        Node* parent = nullptr;
        Node* cur = _root;
        while (cur)
        {
            if (cur->key < key) { parent = cur; cur = cur->right; }
            else if (cur->key > key) { parent = cur; cur = cur->left; }
            else return false;
        }

        cur = new Node(key);
        cur->parent = parent;
        if (parent->key < key) parent->right = cur;
        else parent->left = cur;

        while (parent)
        {
            if (cur == parent->left) --parent->bf;
            else ++parent->bf;

            if (parent->bf == 0)
            {
                break;
            }
            else if (parent->bf == 1 || parent->bf == -1)
            {
                cur = parent;
                parent = parent->parent;
            }
            else if (parent->bf == 2 || parent->bf == -2)
            {
                if (parent->bf == -2 && cur->bf == -1)
                    RotateR(parent);
                else if (parent->bf == 2 && cur->bf == 1)
                    RotateL(parent);
                else if (parent->bf == -2 && cur->bf == 1)
                    RotateLR(parent);
                else if (parent->bf == 2 && cur->bf == -1)
                    RotateRL(parent);
                else
                    assert(false);
                break;
            }
            else
            {
                assert(false);
            }
        }
        return true;
    }

    bool Find(int key) const
    {
        Node* cur = _root;
        while (cur)
        {
            if (key < cur->key) cur = cur->left;
            else if (key > cur->key) cur = cur->right;
            else return true;
        }
        return false;
    }

    bool IsBalance() const
    {
        Node* prev = nullptr;
        return _IsBalance(_root) && _CheckOrder(_root, prev);
    }

private:
    void RotateR(Node* parent)
    {
        ++_rotations;
        Node* subL = parent->left;
        Node* subLR = subL->right;

        parent->left = subLR;
        if (subLR) subLR->parent = parent;

        Node* parentParent = parent->parent;
        subL->right = parent;
        parent->parent = subL;

        if (parentParent == nullptr)
        {
            _root = subL;
            subL->parent = nullptr;
        }
        else
        {
            if (parent == parentParent->left)
                parentParent->left = subL;
            else
                parentParent->right = subL;
            subL->parent = parentParent;
        }

        parent->bf = 0;
        subL->bf = 0;
    }

    void RotateL(Node* parent)
    {
        ++_rotations;
        Node* subR = parent->right;
        Node* subRL = subR->left;

        parent->right = subRL;
        if (subRL) subRL->parent = parent;

        Node* parentParent = parent->parent;
        subR->left = parent;
        parent->parent = subR;

        if (parentParent == nullptr)
        {
            _root = subR;
            subR->parent = nullptr;
        }
        else
        {
            if (parent == parentParent->left)
                parentParent->left = subR;
            else
                parentParent->right = subR;
            subR->parent = parentParent;
        }

        parent->bf = 0;
        subR->bf = 0;
    }

    void RotateLR(Node* parent)
    {
        Node* subL = parent->left;
        Node* subLR = subL->right;
        int bf = subLR->bf;

        RotateL(parent->left);
        RotateR(parent);

        if (bf == 0)
        {
            subL->bf = 0;
            subLR->bf = 0;
            parent->bf = 0;
        }
        else if (bf == -1)
        {
            subL->bf = 0;
            subLR->bf = 0;
            parent->bf = 1;
        }
        else if (bf == 1)
        {
            subL->bf = -1;
            subLR->bf = 0;
            parent->bf = 0;
        }
        else
        {
            assert(false);
        }
    }

    void RotateRL(Node* parent)
    {
        Node* subR = parent->right;
        Node* subRL = subR->left;
        int bf = subRL->bf;

        RotateR(parent->right);
        RotateL(parent);

        if (bf == 0)
        {
            subR->bf = 0;
            subRL->bf = 0;
            parent->bf = 0;
        }
        else if (bf == 1)
        {
            subR->bf = 0;
            subRL->bf = 0;
            parent->bf = -1;
        }
        else if (bf == -1)
        {
            subR->bf = 1;
            subRL->bf = 0;
            parent->bf = 0;
        }
        else
        {
            assert(false);
        }
    }

    int _Height(Node* root) const
    {
        if (root == nullptr) return 0;
        int lh = _Height(root->left);
        int rh = _Height(root->right);
        return lh > rh ? lh + 1 : rh + 1;
    }

    int _Size(Node* root) const
    {
        if (root == nullptr) return 0;
        return _Size(root->left) + _Size(root->right) + 1;
    }

    bool _IsBalance(Node* root) const
    {
        if (root == nullptr) return true;
        int lh = _Height(root->left);
        int rh = _Height(root->right);
        int diff = rh - lh;
        if (diff < -1 || diff > 1) return false;
        if (root->bf != diff) return false;
        return _IsBalance(root->left) && _IsBalance(root->right);
    }

    bool _CheckOrder(Node* root, Node*& prev) const
    {
        if (root == nullptr) return true;
        if (!_CheckOrder(root->left, prev)) return false;
        if (prev && !(prev->key < root->key)) return false;
        prev = root;
        return _CheckOrder(root->right, prev);
    }

    void Destroy(Node* root)
    {
        if (root == nullptr) return;
        Destroy(root->left);
        Destroy(root->right);
        delete root;
    }

    Node* _root;
    int _rotations;
};
}

// ============================================================================
// 三、普通 BST：只用于对照“有序插入退化成单链”
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

    Node* Find(int key) const
    {
        Node* cur = _root;
        while (cur)
        {
            if (key < cur->key) cur = cur->left;
            else if (key > cur->key) cur = cur->right;
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
// 四、实验一：五个最小用例
// ============================================================================
static void test_minimal_cases()
{
    struct Case
    {
        const char* name;
        int a[4];
        int n;
    };
    Case cases[] = {
        { "叔叔为红：只变色，不旋转", { 10, 5, 15, 3 }, 4 },
        { "叔叔黑/空：LL，右单旋 + 变色", { 10, 5, 3 }, 3 },
        { "叔叔黑/空：RR，左单旋 + 变色", { 10, 15, 20 }, 3 },
        { "叔叔黑/空：LR，双旋 + 变色", { 10, 5, 7 }, 3 },
        { "叔叔黑/空：RL，双旋 + 变色", { 10, 15, 13 }, 3 }
    };

    for (const auto& c : cases)
    {
        cout << "--- " << c.name << " ---" << endl;
        rb::RBTree<int, int> t;
        t.SetVerbose(true);
        for (int i = 0; i < c.n; ++i)
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
             << "，单旋=" << t.Rotations()
             << "，变色事件=" << t.Recolors() << endl << endl;
    }
}

// ============================================================================
// 五、实验二：两组课堂序列
// ============================================================================
static void test_course_sequences()
{
    struct Case
    {
        const char* name;
        vector<int> seq;
    };
    Case cases[] = {
        { "常规序列", { 16, 3, 7, 11, 9, 26, 18, 14, 15 } },
        { "双旋序列", { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 } }
    };

    for (const auto& c : cases)
    {
        cout << "--- " << c.name << " ---" << endl;
        rb::RBTree<int, int> t;
        for (int x : c.seq) t.Insert({ x, x });
        cout << "中序: ";
        t.InOrder();
        cout << "高度=" << t.Height() << "，Size=" << t.Size()
             << "，平衡=" << t.IsBalanceTree()
             << "，单旋=" << t.Rotations()
             << "，变色事件=" << t.Recolors() << endl;
        cout << "树形: " << t.DumpInline() << endl << endl;
    }
}

// ============================================================================
// 六、实验三：10 万有序，RB vs AVL vs BST
// ============================================================================
static void test_ordered_100k()
{
    const int N = 100000;
    cout << "N = " << N << "，全部按 1..N 有序插入" << endl;
    using Clock = chrono::high_resolution_clock;

    rb::RBTree<int, int> rbt;
    auto t0 = Clock::now();
    for (int i = 1; i <= N; ++i) rbt.Insert({ i, i });
    auto t1 = Clock::now();
    double rbMs = chrono::duration<double, milli>(t1 - t0).count();
    cout << "RB：高度=" << rbt.Height() << "，Size=" << rbt.Size()
         << "，平衡=" << rbt.IsBalanceTree()
         << "，单旋=" << rbt.Rotations()
         << "，变色事件=" << rbt.Recolors()
         << "，建树=" << fixed << setprecision(1) << rbMs << " ms" << endl;

    avl_cmp::AVLTree avl;
    t0 = Clock::now();
    for (int i = 1; i <= N; ++i) avl.Insert(i);
    t1 = Clock::now();
    double avlMs = chrono::duration<double, milli>(t1 - t0).count();
    cout << "AVL：高度=" << avl.Height() << "，Size=" << avl.Size()
         << "，平衡=" << (avl.IsBalance() ? 1 : 0)
         << "，单旋=" << avl.Rotations()
         << "，建树=" << fixed << setprecision(1) << avlMs << " ms" << endl;

    bst::BSTree b;
    b.BuildChain(N);
    cout << "BST：高度=" << b.Height() << "（有序插入退化成单链，与逐个插入结果一致）" << endl;

    const int Q = 2000;
    vector<int> queries(Q);
    mt19937 rng(2026);
    uniform_int_distribution<int> dist(1, N);
    for (int i = 0; i < Q; ++i) queries[i] = dist(rng);

    int hitRb = 0;
    t0 = Clock::now();
    for (int q : queries) if (rbt.Find(q)) ++hitRb;
    t1 = Clock::now();
    double rbFind = chrono::duration<double, milli>(t1 - t0).count();

    int hitAvl = 0;
    t0 = Clock::now();
    for (int q : queries) if (avl.Find(q)) ++hitAvl;
    t1 = Clock::now();
    double avlFind = chrono::duration<double, milli>(t1 - t0).count();

    int hitBst = 0;
    t0 = Clock::now();
    for (int q : queries) if (b.Find(q)) ++hitBst;
    t1 = Clock::now();
    double bstFind = chrono::duration<double, milli>(t1 - t0).count();

    cout << Q << " 次随机查找：RB 命中 " << hitRb << "，耗时 "
         << fixed << setprecision(3) << rbFind << " ms" << endl;
    cout << Q << " 次随机查找：AVL 命中 " << hitAvl << "，耗时 "
         << avlFind << " ms" << endl;
    cout << Q << " 次随机查找：BST 命中 " << hitBst << "，耗时 "
         << bstFind << " ms" << endl;
    cout << endl;
}

// ============================================================================
// 七、实验四：10 万随机 + 重复 key
// ============================================================================
static void test_random_100k()
{
    const int N = 100000;
    vector<int> v(N);
    for (int i = 0; i < N; ++i) v[i] = i + 1;

    mt19937 rng(2026);
    shuffle(v.begin(), v.end(), rng);

    rb::RBTree<int, int> rbt;
    for (int x : v) rbt.Insert({ x, x });
    cout << "RB 随机插入 " << N << " 个：高度=" << rbt.Height()
         << "，Size=" << rbt.Size()
         << "，平衡=" << rbt.IsBalanceTree()
         << "，单旋=" << rbt.Rotations()
         << "，变色事件=" << rbt.Recolors() << endl;

    avl_cmp::AVLTree avl;
    for (int x : v) avl.Insert(x);
    cout << "AVL 随机插入 " << N << " 个：高度=" << avl.Height()
         << "，Size=" << avl.Size()
         << "，平衡=" << (avl.IsBalance() ? 1 : 0)
         << "，单旋=" << avl.Rotations() << endl;

    int dup = 0;
    for (int x : v) if (!rbt.Insert({ x, x })) ++dup;
    cout << "再插一遍同样的 " << N << " 个：失败 " << dup
         << " 次，Size 仍为 " << rbt.Size() << endl;
}

// ============================================================================
// main：按文章节次顺序跑全部实验
// ============================================================================
int main()
{
    cout << "========== 一、五个最小用例 ==========" << endl;
    test_minimal_cases();

    cout << "========== 二、两组课堂序列 ==========" << endl;
    test_course_sequences();

    cout << "========== 三、10 万有序：RB vs AVL vs BST ==========" << endl;
    test_ordered_100k();

    cout << "========== 四、10 万随机 + 重复 key ==========" << endl;
    test_random_100k();

    return 0;
}

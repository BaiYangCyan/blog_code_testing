// ============================================================================
// list 模拟实现 + 全部实验（单文件版）
// 配套文章：《C++入门篇（十五）：list——带头双向循环链表：接口、迭代器失效与手写模拟实现》
//
// 顶部导航（在编辑器里 Ctrl+F 搜函数名，直达文章对应节）：
//   第三节 3.1  emplace_back 计数实验              → test_emplace_back()
//   第三节 3.2/3.3  独门接口（splice/merge/unique…）→ test_misc_apis()
//   第五节  手写模拟实现 + 全部测试                 → namespace bit + test_*()
//   第六节  效率实测（真实数字需要 -O2）            → test_sort_speed()
//   第七节  erase 错误对照（安全自检，不触发 UB）   → test_erase_bug() / test_erase_ok()
//
// 编译运行（建议 -O2，效率数字才有意义）：
//   g++ -std=c++11 -O2 -Wall -Wextra list_sim.cpp -o list_sim && ./list_sim
// Windows 用户直接双击 build.bat（一键编译 + 运行 + 刷新 output.txt）
// ============================================================================
#include <iostream>
#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>
using namespace std;

// ============================================================================
// 一、bit::list —— 手写模拟实现（第五节 5.1~5.5）
// ============================================================================
namespace bit
{
// ---------------- 1. 结点类 ----------------
template <class T>
struct list_node
{
    T _data;
    list_node<T>* _next;
    list_node<T>* _prev;

    list_node(const T& x = T())
        : _data(x), _next(nullptr), _prev(nullptr)
    {}
};

// ---------------- 2. 正向迭代器：三参数模板 ----------------
template <class T, class Ptr, class Ref>
struct list_iterator
{
    typedef list_node<T> Node;
    typedef list_iterator<T, Ptr, Ref> Self;
    typedef Ptr Pointer;
    typedef Ref Reference;

    Node* _node;

    list_iterator(Node* node)
        : _node(node)
    {}

    Ref operator*()  { return _node->_data; }
    Ptr operator->() { return &_node->_data; }

    Self& operator++()    { _node = _node->_next; return *this; }
    Self  operator++(int) { Self tmp(*this); _node = _node->_next; return tmp; }
    Self& operator--()    { _node = _node->_prev; return *this; }
    Self  operator--(int) { Self tmp(*this); _node = _node->_prev; return tmp; }

    bool operator!=(const Self& it) const { return _node != it._node; }
    bool operator==(const Self& it) const { return _node == it._node; }
};

// ---------------- 3. 反向迭代器：适配器 ----------------
template <class Iterator>
class ReverseListIterator
{
    typedef typename Iterator::Reference Ref;
    typedef typename Iterator::Pointer Ptr;
    typedef ReverseListIterator<Iterator> Self;

public:
    Iterator _it;

    ReverseListIterator(Iterator it)
        : _it(it)
    {}

    Ref operator*()
    {
        Iterator tmp = _it;
        --tmp;            // 先退一格：rbegin() 对应的正向位置是 end()
        return *tmp;
    }
    Ptr operator->() { return &(operator*()); }

    Self& operator++()    { --_it; return *this; }
    Self  operator++(int) { Self tmp(*this); --_it; return tmp; }
    Self& operator--()    { ++_it; return *this; }
    Self  operator--(int) { Self tmp(*this); ++_it; return tmp; }

    bool operator!=(const Self& it) const { return _it != it._it; }
    bool operator==(const Self& it) const { return _it == it._it; }
};

// ---------------- 4. list 类 ----------------
template <class T>
class list
{
    typedef list_node<T> Node;

public:
    typedef list_iterator<T, T*, T&> iterator;
    typedef list_iterator<T, const T*, const T&> const_iterator;
    typedef ReverseListIterator<iterator> reverse_iterator;
    typedef ReverseListIterator<const_iterator> const_reverse_iterator;

    void empty_init()
    {
        _head = new Node;          // 哨兵位头结点
        _head->_next = _head;
        _head->_prev = _head;
        _size = 0;
    }

    list() { empty_init(); }

    list(const list<T>& lt)
    {
        empty_init();
        for (const auto& e : lt)
            push_back(e);
    }

    list(initializer_list<T> il)
    {
        empty_init();
        for (const auto& e : il)
            push_back(e);
    }

    template <class InputIterator>
    list(InputIterator first, InputIterator last)
    {
        empty_init();
        while (first != last)
        {
            push_back(*first);
            ++first;
        }
    }

    list<T>& operator=(list<T> lt)
    {
        swap(lt);
        return *this;
    }

    ~list()
    {
        clear();
        delete _head;
        _head = nullptr;
    }

    void swap(list<T>& lt)
    {
        std::swap(_head, lt._head);
        std::swap(_size, lt._size);
    }

    iterator begin() { return iterator(_head->_next); }
    iterator end()   { return iterator(_head); }
    const_iterator begin() const { return const_iterator(_head->_next); }
    const_iterator end()   const { return const_iterator(_head); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend()   { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }

    bool empty() const { return _size == 0; }
    size_t size() const { return _size; }
    T& front() { return _head->_next->_data; }
    const T& front() const { return _head->_next->_data; }
    T& back() { return _head->_prev->_data; }
    const T& back() const { return _head->_prev->_data; }

    void push_back(const T& x)  { insert(end(), x); }
    void push_front(const T& x) { insert(begin(), x); }
    void pop_back()  { erase(--end()); }
    void pop_front() { erase(begin()); }

    iterator insert(iterator pos, const T& x)
    {
        Node* cur = pos._node;
        Node* prev = cur->_prev;
        Node* newnode = new Node(x);

        prev->_next = newnode;
        newnode->_prev = prev;
        newnode->_next = cur;
        cur->_prev = newnode;

        ++_size;
        return iterator(newnode);
    }

    iterator erase(iterator pos)
    {
        assert(pos != end());       // 哨兵位不能删
        Node* cur = pos._node;
        Node* prev = cur->_prev;
        Node* next = cur->_next;

        prev->_next = next;         // 先重连前后邻居
        next->_prev = prev;
        delete cur;                 // 再删除自己
        --_size;
        return iterator(next);
    }

    void clear()
    {
        iterator it = begin();
        while (it != end())
            it = erase(it);
    }

private:
    Node* _head;
    size_t _size;
};

} // namespace bit

// ============================================================================
// 二、第五节：手写模拟实现全部测试（对应文章 5.6「完整实测」输出）
// ============================================================================
void test_push_pop()
{
    bit::list<int> lt;
    lt.push_back(1); lt.push_back(2); lt.push_back(3); lt.push_back(4);
    lt.push_front(0);
    cout << "[push/pop] forward: ";
    for (auto e : lt) cout << e << " ";
    cout << endl;

    lt.pop_front(); lt.pop_back();
    cout << "[push/pop] after pop: ";
    for (auto e : lt) cout << e << " ";
    cout << "| size=" << lt.size() << " front=" << lt.front() << " back=" << lt.back() << endl;
}

void test_reverse()
{
    bit::list<int> lt{1, 2, 3, 4, 5};
    cout << "[reverse] backward: ";
    for (auto it = lt.rbegin(); it != lt.rend(); ++it)
        cout << *it << " ";
    cout << endl;
}

void test_erase_loop()
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    bit::list<int> l(arr, arr + sizeof(arr) / sizeof(arr[0]));

    // 正确姿势：用 erase 的返回值更新迭代器
    auto it = l.begin();
    while (it != l.end())
    {
        if (*it % 2 == 0)
            it = l.erase(it);
        else
            ++it;
    }
    cout << "[erase even] data: ";
    for (auto e : l) cout << e << " ";
    cout << "| size=" << l.size() << endl;
}

void test_copy_assign()
{
    bit::list<int> a{1, 2, 3};
    bit::list<int> b(a);      // 拷贝构造
    bit::list<int> c;
    c = a;                    // 赋值重载
    b.push_back(4);

    cout << "[copy/assign] a=";
    for (auto e : a) cout << e << " ";
    cout << "| b=";
    for (auto e : b) cout << e << " ";
    cout << "| c=";
    for (auto e : c) cout << e << " ";
    cout << endl;
}

void test_string()
{
    bit::list<std::string> ls;
    ls.push_back("1111");
    ls.push_back("2222");
    ls.push_front("0000");
    cout << "[string list] data: ";
    for (const auto& e : ls) cout << e << " ";
    cout << endl;
}

template <class Container>
void print_container(const Container& con)
{
    typename Container::const_iterator it = con.begin();
    while (it != con.end())
    {
        cout << *it << " ";
        ++it;
    }
    cout << endl;
}

void test_const_print()
{
    const bit::list<int> lc{1, 2, 3};   // const 对象，只能走 const 迭代器
    cout << "[const print] data: ";
    print_container(lc);
}

// ============================================================================
// 三、第三节 3.1：emplace_back 计数实验
// ============================================================================
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

static void reset(const char* tag)
{
    Track::ctor = Track::copy = Track::move = 0;
    cout << tag;
}

static void report()
{
    cout << "构造=" << Track::ctor << "，拷贝=" << Track::copy << "，移动=" << Track::move << endl;
}

void test_emplace_back()
{
    list<Track> lt;

    Track named(1);                    // 这行在计数窗口之外

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
}

// ============================================================================
// 四、第三节 3.2 / 3.3：独门接口（splice / merge / unique / remove / reverse / swap / find）
// ============================================================================
template <class T>
void print_list(const char* tag, const list<T>& lt)
{
    cout << tag;
    for (const auto& e : lt) cout << e << " ";
    cout << "| size=" << lt.size() << endl;
}

void test_misc_apis()
{
    // splice：剪切，不是粘贴
    cout << "=== splice 整链剪切 ===" << endl;
    {
        list<int> l1{1, 2, 3, 4};
        list<int> l2{10, 20, 30};
        auto it = ++l1.begin();          // 指向 2
        l1.splice(it, l2);               // 把 l2 整条剪到 2 前面
        print_list("l1: ", l1);
        print_list("l2: ", l2);
        cout << "it 仍指向 2，解引用 = " << *it << endl;
    }

    cout << endl << "=== splice 自剪切（把第一个结点剪到末尾） ===" << endl;
    {
        list<int> l1{1, 2, 3, 4};
        l1.splice(l1.end(), l1, l1.begin());
        print_list("l1: ", l1);
    }

    cout << endl << "=== merge ===" << endl;
    {
        list<int> a{1, 3, 5};
        list<int> b{2, 4, 6};
        a.merge(b);
        print_list("a: ", a);
        print_list("b: ", b);
    }

    cout << endl << "=== unique ===" << endl;
    {
        list<int> l{1, 1, 2, 2, 3, 1, 1};
        l.unique();
        print_list("直接 unique: ", l);       // 末尾的 1 还在
        l.sort();
        l.unique();
        print_list("先 sort 再 unique: ", l);
    }

    cout << endl << "=== remove ===" << endl;
    {
        list<int> l{1, 2, 3, 2, 4};
        l.remove(2);
        print_list("remove(2): ", l);
    }

    cout << endl << "=== reverse ===" << endl;
    {
        list<int> l{1, 2, 3, 4, 5};
        l.reverse();
        print_list("reverse: ", l);
    }

    cout << endl << "=== swap 与 std::find ===" << endl;
    {
        list<int> a{1, 2, 3};
        list<int> b{7, 8};
        a.swap(b);                       // 只换哨兵指针，O(1)
        print_list("a: ", a);
        print_list("b: ", b);
        auto f = std::find(a.begin(), a.end(), 8);   // list 没有成员 find
        if (f != a.end())
            cout << "std::find 找到 8" << endl;
    }
}

// ============================================================================
// 五、第七节：erase 错误对照（安全自检版，不触发 UB）
// ============================================================================
namespace demo
{
template <class T>
struct Node
{
    T data;
    Node* prev;
    Node* next;
    Node(const T& d = T()) : data(d), prev(nullptr), next(nullptr) {}
};

template <class T>
class List
{
    Node<T>* head;

public:
    List()
    {
        head = new Node<T>;
        head->prev = head;
        head->next = head;
    }

    ~List()
    {
        Node<T>* p = head->next;
        while (p != head)
        {
            Node<T>* next = p->next;
            delete p;
            p = next;
        }
        delete head;
    }

    void push_back(const T& x)
    {
        Node<T>* node = new Node<T>(x);
        Node<T>* tail = head->prev;
        tail->next = node;
        node->prev = tail;
        node->next = head;
        head->prev = node;
    }

    Node<T>* find(const T& x)
    {
        Node<T>* p = head->next;
        while (p != head)
        {
            if (p->data == x) return p;
            p = p->next;
        }
        return nullptr;
    }

    // 错误版：只改被删结点自己的指针，没有重连前后邻居
    // （这里故意不 delete，方便安全自检；真实代码加上 delete 就是悬空指针）
    void erase_bug(Node<T>* cur)
    {
        Node<T>* prev = cur->prev;
        Node<T>* next = cur->next;
        cur->next = next;   // 没用的赋值
        cur->prev = prev;   // 没用的赋值
        // delete cur;      // 真实代码在这里删除：prev->next / next->prev 随即悬空
        detached = cur;
    }

    // 正确版：先重连前后邻居，再删自己
    void erase_ok(Node<T>* cur)
    {
        Node<T>* prev = cur->prev;
        Node<T>* next = cur->next;
        prev->next = next;
        next->prev = prev;
        delete cur;
    }

    // 安全自检：只读取"存活节点"的指针值，不解引用被删节点
    bool check()
    {
        if (detached)
        {
            if (detached->prev->next == detached) return false;
            if (detached->next->prev == detached) return false;
        }
        Node<T>* cur = head;
        int guard = 0;
        do
        {
            if (cur->next->prev != cur) return false;
            cur = cur->next;
            if (++guard > 100) return false;
        } while (cur != head);
        return true;
    }

    void print()
    {
        Node<T>* p = head->next;
        while (p != head)
        {
            cout << p->data << " ";
            p = p->next;
        }
        cout << endl;
    }

    size_t size()
    {
        size_t n = 0;
        Node<T>* p = head->next;
        while (p != head)
        {
            ++n;
            p = p->next;
        }
        return n;
    }

private:
    Node<T>* detached = nullptr;   // 记录错误版摘下的节点，仅用于演示
};
}

void test_erase_bug()
{
    cout << "=== 错误版 erase（未重连前后指针） ===" << endl;
    demo::List<int> l;
    for (int i = 1; i <= 4; ++i) l.push_back(i);
    l.erase_bug(l.find(3));
    cout << "双链自检: " << (l.check() ? "通过" : "失败——存在悬空链接") << endl;
    cout << "原因：2->next 仍指向被摘下的 3，4->prev 同理；一旦 delete，"
         << "后续遍历/删除就会访问已释放内存（UB）" << endl;
}

void test_erase_ok()
{
    cout << endl << "=== 正确版 erase（先重连，再删除） ===" << endl;
    demo::List<int> l;
    for (int i = 1; i <= 4; ++i) l.push_back(i);
    l.erase_ok(l.find(3));
    cout << "双链自检: " << (l.check() ? "通过" : "失败") << endl;
    cout << "size=" << l.size() << "，内容: ";
    l.print();
}

// ============================================================================
// 六、第六节：效率实测（list.sort vs 拷贝到 vector 排序）
// ============================================================================
void test_sort_speed()
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
    std::vector<int> v(lt2.begin(), lt2.end());
    std::sort(v.begin(), v.end());
    lt2.assign(v.begin(), v.end());
    clock_t e2 = clock();

    cout << "N = " << N << "（Release/-O2 环境实测）" << endl;
    cout << "list.sort()                  : " << (double)(e1 - b1) / CLOCKS_PER_SEC << " s" << endl;
    cout << "copy -> vector sort -> back  : " << (double)(e2 - b2) / CLOCKS_PER_SEC << " s" << endl;
}

// ============================================================================
// main：按文章节次顺序跑全部实验
// ============================================================================
int main()
{
    cout << "========== 第三节 3.1 emplace_back 计数实验 ==========" << endl;
    test_emplace_back();

    cout << endl << "========== 第三节 3.2 / 3.3 独门接口 ==========" << endl;
    test_misc_apis();

    cout << endl << "========== 第五节 手写模拟实现全部测试 ==========" << endl;
    test_push_pop();
    test_reverse();
    test_erase_loop();
    test_copy_assign();
    test_string();
    test_const_print();
    cout << "ALL TESTS PASSED" << endl;

    cout << endl << "========== 第七节 erase 错误对照（安全自检） ==========" << endl;
    test_erase_bug();
    test_erase_ok();

    cout << endl << "========== 第六节 效率实测（真实数字需 -O2） ==========" << endl;
    test_sort_speed();

    return 0;
}

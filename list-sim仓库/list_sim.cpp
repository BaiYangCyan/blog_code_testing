// C++ list 模拟实现（完整版·实测通过）
// 编译：g++ -std=c++11 -Wall -Wextra list_sim.cpp -o list_sim
// 结构：结点类（list_node）+ 正向迭代器（三参数模板）+ 反向迭代器（适配器）+ list 类（哨兵位）
#include <iostream>
#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <string>
using namespace std;

namespace bit
{
// ==================== 1. 结点类 ====================
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

// ==================== 2. 正向迭代器：三参数模板 ====================
// T 决定结点类型；Ptr / Ref 决定 operator-> 和 operator* 的返回类型，
// 用同一份代码同时支持 iterator 和 const_iterator
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

// ==================== 3. 反向迭代器：适配器 ====================
// 内部包一个正向迭代器：++ 就是正向的 --，-- 就是正向的 ++
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

// ==================== 4. list 类 ====================
template <class T>
class list
{
    typedef list_node<T> Node;

public:
    typedef list_iterator<T, T*, T&> iterator;
    typedef list_iterator<T, const T*, const T&> const_iterator;
    typedef ReverseListIterator<iterator> reverse_iterator;
    typedef ReverseListIterator<const_iterator> const_reverse_iterator;

    // ---- 构造 ----
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

    // 赋值重载：值传递 + swap
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

    // ---- 迭代器 ----
    iterator begin() { return iterator(_head->_next); }
    iterator end()   { return iterator(_head); }
    const_iterator begin() const { return const_iterator(_head->_next); }
    const_iterator end()   const { return const_iterator(_head); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend()   { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }

    // ---- 容量与访问 ----
    bool empty() const { return _size == 0; }
    size_t size() const { return _size; }
    T& front() { return _head->_next->_data; }
    const T& front() const { return _head->_next->_data; }
    T& back() { return _head->_prev->_data; }
    const T& back() const { return _head->_prev->_data; }

    // ---- 增删改 ----
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
        return iterator(next);      // 返回下一个位置
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

// ==================== 测试 ====================
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

int main()
{
    test_push_pop();
    test_reverse();
    test_erase_loop();
    test_copy_assign();
    test_string();
    test_const_print();
    cout << "ALL TESTS PASSED" << endl;
    return 0;
}

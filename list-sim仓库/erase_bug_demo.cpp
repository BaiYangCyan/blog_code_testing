// 常见错误对照：erase 只改被删节点自己的指针，没有重连前后邻居
// 编译：g++ erase_bug_demo.cpp -o erase_bug_demo
// 说明：错误版故意不 delete，用"安全自检"展示结构断链；
//       真实代码一旦 delete，这些悬空指针就会在后续遍历时访问已释放内存（UB）
#include <iostream>
using namespace std;

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

    // ---------------- 错误版 erase ----------------
    // 只又"翻译"了一遍 cur 自己的 next/prev（本来就是对的），
    // 没有把 prev->next 和 next->prev 接起来。
    // 这里故意不 delete，方便做结构自检；真实代码加 delete 后就是悬空指针。
    void erase_bug(Node<T>* cur)
    {
        Node<T>* prev = cur->prev;
        Node<T>* next = cur->next;
        cur->next = next;   // 没用的赋值
        cur->prev = prev;   // 没用的赋值
        // delete cur;      // 真实代码在这里删除：prev->next / next->prev 随即悬空
        detached = cur;
    }

    // ---------------- 正确版 erase ----------------
    void erase_ok(Node<T>* cur)
    {
        Node<T>* prev = cur->prev;
        Node<T>* next = cur->next;
        prev->next = next;  // 前驱接后继
        next->prev = prev;  // 后继接前驱
        delete cur;
    }

    // 安全自检：只读取"存活节点"的指针值，不解引用被删节点
    bool check()
    {
        if (detached)
        {
            // 错误版：前驱的 next 仍指向已摘下的节点，后继的 prev 同理
            if (detached->prev->next == detached) return false;
            if (detached->next->prev == detached) return false;
        }
        // 正确版：整环双向一致性检查
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

int main()
{
    cout << "=== 错误版 erase（未重连前后指针） ===" << endl;
    {
        demo::List<int> l;
        for (int i = 1; i <= 4; ++i) l.push_back(i);
        l.erase_bug(l.find(3));
        cout << "双链自检: " << (l.check() ? "通过" : "失败——存在悬空链接") << endl;
        cout << "原因：2->next 仍指向被摘下的 3，4->prev 同理；一旦 delete，"
             << "后续遍历/删除就会访问已释放内存（UB）" << endl;
    }

    cout << endl << "=== 正确版 erase（先重连，再删除） ===" << endl;
    {
        demo::List<int> l;
        for (int i = 1; i <= 4; ++i) l.push_back(i);
        l.erase_ok(l.find(3));
        cout << "双链自检: " << (l.check() ? "通过" : "失败") << endl;
        cout << "size=" << l.size() << "，内容: ";
        l.print();
    }
    return 0;
}

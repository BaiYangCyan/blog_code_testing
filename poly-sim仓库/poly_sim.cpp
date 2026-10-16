// ============================================================================
// 多态 一站式实验（单文件版）
// 配套文章：《C++进阶篇（二）：多态——虚函数、重写、虚函数表与三个经典坑》
//
// 顶部导航（在编辑器里 Ctrl+F 搜函数名，直达文章对应节）：
//   第一节  多态初体验：买票                  → test_poly_basic()
//   第二节  构成条件：引用版 vs 值传递版      → test_ref_vs_value()
//   第三节  重写细节：override / 协变         → test_override_covariant()
//   第四节  重载 / 重写 / 隐藏 三兄弟         → test_three_concepts()
//   第五节  虚析构：delete 基类指针           → test_virtual_dtor()
//   第六节  抽象类与纯虚函数                  → test_abstract()
//   第七节  原理探针：虚表与 sizeof           → test_vtable_probe()
//   第八节  面试小灶：默认参数 + 构造中调虚   → test_interview_traps()
//
// 编译运行：
//   g++ -std=c++11 -O2 -Wall -Wextra poly_sim.cpp -o poly_sim && ./poly_sim
// Windows 用户直接双击 build.bat（一键编译 + 运行 + 刷新 output.txt）
// ============================================================================
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

// ============================================================================
// 第一节：多态初体验——买票
// ============================================================================
namespace sec1
{
class Person
{
public:
    virtual void BuyTicket() { cout << "买票-全价" << endl; }
};

class Student : public Person
{
public:
    // 重写基类虚函数（这里不写 virtual 也构成重写，但规范写法建议写上）
    void BuyTicket() override { cout << "买票-打折" << endl; }
};

class Soldier : public Person
{
public:
    void BuyTicket() override { cout << "买票-优先" << endl; }
};

void Func(Person& ptr)   // 基类的"引用"调用
{
    // 虽然都是 Person 引用在调用，但到底调谁，由 ptr 引用的对象决定
    ptr.BuyTicket();
}
}

void test_poly_basic()
{
    sec1::Person ps;
    sec1::Student st;
    sec1::Soldier sd;

    sec1::Func(ps);
    sec1::Func(st);
    sec1::Func(sd);
}

// ============================================================================
// 第二节：构成条件对照——引用版 vs 值传递版
// ============================================================================
namespace sec2
{
class Person
{
public:
    virtual void BuyTicket() { cout << "买票-全价" << endl; }
};

class Student : public Person
{
public:
    void BuyTicket() override { cout << "买票-打折" << endl; }
};

void FuncRef(Person& ptr)   // 引用：构成多态
{
    ptr.BuyTicket();
}

void FuncValue(Person ptr)  // 值传递：发生切片，不构成多态
{
    ptr.BuyTicket();
}
}

void test_ref_vs_value()
{
    sec2::Person ps;
    sec2::Student st;

    cout << "[引用版  ] ";
    sec2::FuncRef(ps);
    cout << "[引用版  ] ";
    sec2::FuncRef(st);

    cout << "[值传递版] ";
    sec2::FuncValue(ps);
    cout << "[值传递版] ";
    sec2::FuncValue(st);
    cout << "（值传递把 st 切成了 Person，多态消失）" << endl;
}

// ============================================================================
// 第三节：重写细节——override 与协变
// ============================================================================
namespace sec3
{
class Car
{
public:
    virtual void Drive() { cout << "Car::Drive" << endl; }
};

class Benz : public Car
{
public:
    // override：让编译器帮你检查是不是真的重写上了（写错函数名/参数会直接报错）
    void Drive() override { cout << "Benz-舒适" << endl; }
};

class Derive final : public Car   // final 类：不能被继承（上一章讲过）
{
public:
    void Drive() override { cout << "Derive-稳健" << endl; }
};

// ---- 协变：返回值不同类型，但类型之间是父子类关系 ----
class A {};
class B : public A {};

class PBase
{
public:
    virtual A* BuyTicket()
    {
        cout << "买票-全价（返回 A*）" << endl;
        return nullptr;
    }
};

class PStudent : public PBase
{
public:
    B* BuyTicket() override   // 返回值协变：B* 也是一种 A*
    {
        cout << "买票-打折（返回 B*）" << endl;
        return nullptr;
    }
};
}

void test_override_covariant()
{
    sec3::Benz bz;
    sec3::Car* c1 = &bz;
    c1->Drive();

    sec3::PBase pbase;
    sec3::PStudent pstudent;
    sec3::PBase* p1 = &pbase;
    sec3::PBase* p2 = &pstudent;
    p1->BuyTicket();
    p2->BuyTicket();
}

// ============================================================================
// 第四节：重载 / 重写 / 隐藏
// ============================================================================
namespace sec4
{
// 重载：同作用域，函数名相同，参数不同
class Calc
{
public:
    int Add(int a, int b) { return a + b; }
    double Add(double a, double b) { return a + b; }
};

// 重写：父类子类不同作用域，函数名 / 参数 / 返回值相同（协变例外），都是虚函数
class Base
{
public:
    virtual void func() { cout << "Base::func（被重写）" << endl; }
    void other() { cout << "Base::other" << endl; }
};

class Derive : public Base
{
public:
    void func() override { cout << "Derive::func" << endl; }
    // 隐藏：同名但参数不同（不构成重写），基类版本被隐藏
    void other(int x) { cout << "Derive::other(int): " << x << endl; }
};

// 隐藏：成员变量同名也算隐藏
class P2
{
protected:
    int _num = 111;
};

class S2 : public P2
{
public:
    void Print()
    {
        cout << "基类 _num=" << P2::_num << "，派生 _num=" << _num << "（变量同名隐藏）" << endl;
    }
protected:
    int _num = 999;
};
}

void test_three_concepts()
{
    sec4::Calc c;
    cout << "[重载] " << c.Add(1, 2) << " / " << c.Add(1.5, 2.5) << endl;

    sec4::Derive d;
    sec4::Base* pb = &d;
    pb->func();          // 多态：调 Derive::func
    d.other(10);         // 调自己的版本
    d.Base::other();     // 基类版本被隐藏，要指定类域

    sec4::S2 s2;
    s2.Print();
}

// ============================================================================
// 第五节：虚析构——为什么基类析构要加 virtual
// ============================================================================
namespace sec5
{
// ---- 反例：析构不虚 ----
class A_nv
{
public:
    ~A_nv() { cout << "  ~A_nv()" << endl; }
};

class B_nv : public A_nv
{
public:
    ~B_nv()
    {
        cout << "  ~B_nv()->delete:" << (void*)_p << endl;
        delete[] _p;
    }
protected:
    int* _p = new int[10];   // 需要释放的资源
};

// ---- 正例：析构加 virtual ----
class A_v
{
public:
    virtual ~A_v() { cout << "  ~A_v()" << endl; }
};

class B_v : public A_v
{
public:
    ~B_v() override
    {
        cout << "  ~B_v()->delete:" << (void*)_p << endl;
        delete[] _p;
    }
protected:
    int* _p = new int[10];
};
}

void test_virtual_dtor()
{
    cout << "--- 反例：~A 不虚，delete 基类指针 ---" << endl;
    sec5::A_nv* p1 = new sec5::B_nv;
    delete p1;
    cout << " （~B_nv() 根本没被调用：_p 指向的堆内存泄漏）" << endl;

    cout << "--- 正例：~A 加 virtual，delete 基类指针 ---" << endl;
    sec5::A_v* p2 = new sec5::B_v;
    delete p2;
    cout << " （先析构派生、再析构基类，资源正确释放）" << endl;
}

// ============================================================================
// 第六节：抽象类与纯虚函数
// ============================================================================
namespace sec6
{
class Car
{
public:
    virtual void Drive() const = 0;   // 纯虚函数：没有实现（=0）
};

class Benz : public Car
{
public:
    void Drive() const override { cout << "Benz-舒适" << endl; }
};

class BMW : public Car
{
public:
    void Drive() const override { cout << "BMW-操控" << endl; }
};

class WENJIE : public Car
{
public:
    void Drive() const override { cout << "WENJIE-智能" << endl; }
};

void Func(const Car& p)
{
    p.Drive();
}
}

void test_abstract()
{
    // Car car;   // 编译报错 C2259：无法实例化抽象类
    sec6::Func(sec6::Benz());
    sec6::Func(sec6::BMW());
    sec6::Func(sec6::WENJIE());
}

// ============================================================================
// 第七节：原理探针——虚表与 sizeof
// ============================================================================
namespace sec7
{
struct NoVirt
{
    int _b = 1;
    char _ch = 'x';
};

struct HasVirt
{
    virtual void Func1() {}
    int _b = 1;
    char _ch = 'x';
};

class Person
{
public:
    virtual void BuyTicket() { cout << "买票-全价" << endl; }
    string _name = "张三";
};

class Student : public Person
{
public:
    void BuyTicket() override { cout << "买票-打折" << endl; }
    string _id = "2026";
};

// ---- 多继承：一个对象挂两张虚表 ----
class MBase1
{
public:
    virtual void func1() { cout << "MBase1::func1" << endl; }
    virtual void func2() { cout << "MBase1::func2" << endl; }
protected:
    int _a1 = 1;
};

class MBase2
{
public:
    virtual void func1() { cout << "MBase2::func1" << endl; }
    virtual void func2() { cout << "MBase2::func2" << endl; }
protected:
    int _a2 = 2;
};

class MDerive : public MBase1, public MBase2
{
public:
    void func1() override { cout << "MDerive::func1" << endl; }
    virtual void func3() { cout << "MDerive::func3（新虚函数）" << endl; }
protected:
    int _b = 3;
};

// ---- sizeof 变体（64 位下观察 vfptr 与对齐）----
struct VInt3
{
    virtual void f() {}
    int _a = 1, _b2 = 2, _c = 3;
};

struct VChar
{
    virtual void f() {}
    char _c = 'x';
};
}

// 安全地读取对象头 8 字节（vfptr），用 memcpy 避开类型双关问题
void* vfptr_of(const void* obj)
{
    void* p = nullptr;
    memcpy(&p, obj, sizeof(void*));
    return p;
}

// ---- 构造 / 析构中调用虚函数：不构成多态 ----
namespace sec7b
{
class Base
{
public:
    Base()  { cout << "  Base 构造中调用 func()："; func(); }
    virtual void func() { cout << "Base::func（虚表还没切到派生类）" << endl; }
    virtual ~Base() {}
};

class Derived : public Base
{
public:
    void func() override { cout << "Derived::func" << endl; }
};
}

void test_vtable_probe()
{
    cout << "sizeof(NoVirt)  = " << sizeof(sec7::NoVirt) << " 字节（无虚函数）" << endl;
    cout << "sizeof(HasVirt) = " << sizeof(sec7::HasVirt) << " 字节（有虚函数，多出一个 vfptr）" << endl;
    cout << "sizeof(VInt3)   = " << sizeof(sec7::VInt3) << " 字节（vfptr + 3 个 int）" << endl;
    cout << "sizeof(VChar)   = " << sizeof(sec7::VChar) << " 字节（vfptr + 1 个 char，按 8 字节对齐）" << endl;

    sec7::Student s1, s2;
    sec7::Person p1;
    cout << "vfptr(s1) == vfptr(s2) ? " << (vfptr_of(&s1) == vfptr_of(&s2))
         << "（同类型的对象共用一张虚表）" << endl;
    cout << "vfptr(p1) == vfptr(s1) ? " << (vfptr_of(&p1) == vfptr_of(&s1))
         << "（各类型各有独立的虚表）" << endl;

    cout << "--- 构造函数里调虚函数 ---" << endl;
    sec7b::Derived d;    // 构造期间：调的是 Base::func
    d.func();            // 对象完整之后：才轮到 Derived::func
}

// ============================================================================
// 第七节附：多继承虚表 + 虚表住哪
// ============================================================================
void test_multi_inherit_vtable()
{
    sec7::MDerive d;

    sec7::MBase2* pb2 = &d;                       // 隐式转换：编译器算好 Base2 子对象偏移
    void* vptr1 = vfptr_of(&d);                   // Base1 部分的虚表
    void* vptr2 = vfptr_of(pb2);                  // Base2 部分的虚表
    ptrdiff_t offset = (char*)pb2 - (char*)&d;    // Base2 子对象在对象里的偏移

    cout << "vptr1（Base1 部分）= " << vptr1 << endl;
    cout << "vptr2（Base2 部分）= " << vptr2 << "，偏移 " << offset << " 字节" << endl;
    cout << "两张虚表地址不同: " << (vptr1 != vptr2) << endl;

    void** vt1 = (void**)vptr1;
    void** vt2 = (void**)vptr2;
    cout << "vt1: slot0=" << vt1[0] << "  slot1=" << vt1[1] << "  slot2=" << vt1[2] << endl;
    cout << "vt2: slot0=" << vt2[0] << "  slot1=" << vt2[1] << "  slot2=" << vt2[2] << endl;
    cout << "vt1[0] == vt2[0] ? " << (vt1[0] == vt2[0])
         << "（第二张表里的覆盖函数常是 thunk：先修正 this 偏移再跳转）" << endl;
    cout << "vt1[1] != vt2[1] ? " << (vt1[1] != vt2[1]) << "（两边的 func2 各留各的版本）" << endl;

    // 通过两个基类指针调用：都走派生类覆盖后的 func1
    sec7::MBase1* p1 = &d;
    p1->func1();
    pb2->func1();
}

void test_vtable_addr()
{
    static int s = 1;
    int st = 0;
    int* hp = new int(1);
    const char* lit = "xxxxxxxx";
    sec7::Person p;

    cout << "栈    : " << (void*)&st << endl;
    cout << "静态区: " << (void*)&s << endl;
    cout << "堆    : " << hp << endl;
    cout << "常量区: " << (void*)lit << endl;
    cout << "虚表  : " << vfptr_of(&p) << "（数值上贴近常量区——C++ 标准没规定虚表存哪，这是编译器实现细节）" << endl;
    delete hp;
}

// ============================================================================
// 第八节：面试小灶——默认参数坑
// ============================================================================
namespace sec8
{
class A
{
public:
    virtual void func(int val = 1) { cout << "A->" << val << endl; }
    virtual void test() { func(); }
};

class B : public A
{
public:
    void func(int val = 0) override { cout << "B->" << val << endl; }
};
}

void test_interview_traps()
{
    sec8::B b;
    sec8::B* p = &b;
    p->test();      // 经典题：输出 B->1

    b.func();       // 直接调用：默认参数看 B 自己的 → B->0
    sec8::A* pa = &b;
    pa->func();     // 多态调用：默认参数看 A 的 → B->1
    cout << "（函数体动态绑定走 B 的实现；默认参数静态绑定看编译时的类型）" << endl;
}

// ============================================================================
// main：按文章节次顺序跑全部实验
// ============================================================================
int main()
{
    cout << "========== 第一节 多态初体验：买票 ==========" << endl;
    test_poly_basic();

    cout << endl << "========== 第二节 构成条件：引用版 vs 值传递版 ==========" << endl;
    test_ref_vs_value();

    cout << endl << "========== 第三节 重写细节：override / 协变 ==========" << endl;
    test_override_covariant();

    cout << endl << "========== 第四节 重载 / 重写 / 隐藏 三兄弟 ==========" << endl;
    test_three_concepts();

    cout << endl << "========== 第五节 虚析构：delete 基类指针 ==========" << endl;
    test_virtual_dtor();

    cout << endl << "========== 第六节 抽象类与纯虚函数 ==========" << endl;
    test_abstract();

    cout << endl << "========== 第七节 原理探针：虚表与 sizeof ==========" << endl;
    test_vtable_probe();
    test_multi_inherit_vtable();
    test_vtable_addr();

    cout << endl << "========== 第八节 面试小灶：默认参数坑 ==========" << endl;
    test_interview_traps();

    return 0;
}

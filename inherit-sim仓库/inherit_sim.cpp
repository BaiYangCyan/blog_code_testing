// ============================================================================
// 继承 一站式实验（单文件版）
// 配套文章：《C++进阶篇（一）：继承——权限、切片、默认成员函数与菱形继承》
//
// 顶部导航（在编辑器里 Ctrl+F 搜函数名，直达文章对应节）：
//   第一节  基础继承与访问权限            → test_inherit_basic()
//   第二节  继承类模板（stack : vector）  → test_inherit_template()
//   第三节  赋值兼容与对象切片            → test_convert_slice()
//   第四节  作用域与隐藏                  → test_hiding()
//   第五节  默认成员函数全链路            → test_default_members()
//   第五节  不能被继承的类（final）       → test_final()
//   第六节  友元与静态成员                → test_friend_static()
//   第七节  多继承、菱形继承与虚继承      → test_diamond()
//   第八节  继承与组合                    → test_compose()
//
// 编译运行：
//   g++ -std=c++11 -O2 -Wall -Wextra inherit_sim.cpp -o inherit_sim && ./inherit_sim
// Windows 用户直接双击 build.bat（一键编译 + 运行 + 刷新 output.txt）
// ============================================================================
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ============================================================================
// 第一节：基础继承与访问权限（Person / Student / Teacher）
// ============================================================================
namespace basic
{
class Person
{
public:
    void identity() { cout << "identity(): " << _name << " / " << _age << endl; }

protected:
    string _name = "张三";   // 姓名（protected：派生类里能直接访问）
    string _address;         // 地址
    string _tel;             // 电话

private:
    int _age = 18;           // 年龄（private：派生类里"看得见摸不着"）
};

class Student : public Person
{
public:
    void study()
    {
        // _name 是 protected：派生类内部可以直接用
        cout << "study(): " << _name << " 正在学习" << endl;
        // cout << _age << endl;  // 编译报错：private 成员在派生类中不可见
    }

protected:
    int _stuid = 2026;   // 学号
};

class Teacher : public Person
{
public:
    void teaching() { cout << "teaching(): " << _name << " 正在授课" << endl; }

protected:
    string _title = "助教";   // 职称
};
}

void test_inherit_basic()
{
    basic::Student s;
    basic::Teacher t;
    s.identity();     // 基类的 public 成员，派生类对象可以直接调
    s.study();
    t.identity();
    t.teaching();
    cout << "（_name 在派生类内部可见：protected；_age 不可见：private）" << endl;
}

// ============================================================================
// 第二节：继承类模板——stack 是 vector 的"一种"
// ============================================================================
namespace tpl
{
// stack 和 vector 的关系：既符合 is-a（是一种），也符合 has-a（有一个）
template <class T>
class stack : public vector<T>
{
public:
    void push(const T& x)
    {
        // 基类是类模板时，要指定类域，否则报错：
        // error C3861: "push_back": 找不到标识符（按需实例化惹的祸）
        vector<T>::push_back(x);
    }
    void pop() { vector<T>::pop_back(); }
    T& top()   { return vector<T>::back(); }
    bool empty() { return vector<T>::empty(); }
};
}

void test_inherit_template()
{
    tpl::stack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);
    cout << "stack: ";
    while (!st.empty())
    {
        cout << st.top() << " ";
        st.pop();
    }
    cout << "| 复用基类的 size(): " << st.size() << endl;
}

// ============================================================================
// 第三节：赋值兼容与对象切片
// ============================================================================
namespace conv
{
class Person
{
public:
    Person(const string& name = "peter") : _name(name) {}
    string _name;
};

class Student : public Person
{
public:
    Student(const string& name, int no) : Person(name), _No(no) {}
    int _No;
};
}

void test_convert_slice()
{
    conv::Student sobj("jack", 18);

    // 1. 派生类对象可以赋值给基类的指针 / 引用（绑定到"基类那部分"）
    conv::Person* pp = &sobj;
    conv::Person& rp = sobj;
    cout << "指针看基类部分: " << pp->_name << " | 引用看基类部分: " << rp._name << endl;

    // 2. 派生类对象赋值给基类对象 → 调用基类拷贝构造，发生"切片"
    conv::Person pobj = sobj;
    cout << "切片后 pobj: " << pobj._name << "（_No 被切掉了）" << endl;
    cout << "sobj 不受影响: " << sobj._name << " / _No=" << sobj._No << endl;

    // 3. 基类对象不能赋值给派生类对象，下面这行编译报错：
    // sobj = pobj;

    // 4. 基类指针/引用强转给派生类，需要它真的指向派生类对象（dynamic_cast 属于 RTTI，后续章节）
}

// ============================================================================
// 第四节：作用域与隐藏
// ============================================================================
namespace hide
{
class Person
{
protected:
    string _name = "小李子";
    int _num = 111;   // 身份证号
};

class Student : public Person
{
public:
    void Print()
    {
        cout << " 姓名:" << _name << endl;
        cout << " 身份证号:" << Person::_num << endl;   // 指定基类作用域
        cout << " 学号:" << _num << endl;               // 默认访问自己的
    }
protected:
    int _num = 999;   // 学号（和基类同名 → 隐藏）
};

class A
{
public:
    void fun() { cout << "A::fun()" << endl; }
};

class B : public A
{
public:
    void fun(int i) { cout << "B::fun(int i): " << i << endl; }
};
}

void test_hiding()
{
    hide::Student s1;
    s1.Print();     // 111 和 999 各归各的

    hide::B b;
    b.fun(10);      // 派生类自己的版本
    // b.fun();     // 编译报错：基类 fun() 被隐藏（同名即隐藏，与参数无关）
    b.A::fun();     // 指定类域才能调用基类版本
    cout << "（成员函数隐藏只看函数名，不看参数列表）" << endl;
}

// ============================================================================
// 第五节：默认成员函数全链路（构造/拷贝/赋值/析构）
// ============================================================================
namespace dm
{
class Person
{
public:
    Person(const char* name = "peter")
        : _name(name)
    {
        cout << "  Person(const char*)  构造：" << _name << endl;
    }

    Person(const Person& p)
        : _name(p._name)
    {
        cout << "  Person(const Person&) 拷贝构造" << endl;
    }

    Person& operator=(const Person& p)
    {
        cout << "  Person::operator=(const Person&) 赋值" << endl;
        if (this != &p)
            _name = p._name;
        return *this;
    }

    ~Person()
    {
        cout << "  ~Person() 析构" << endl;
    }

protected:
    string _name;
};

class Student : public Person
{
public:
    Student(const char* name, int num)
        : Person(name)        // 必须显式调用基类构造（没有默认构造时）
        , _num(num)
    {
        cout << "  Student(const char*, int) 构造" << endl;
    }

    Student(const Student& s)
        : Person(s)           // 调用基类拷贝构造
        , _num(s._num)
    {
        cout << "  Student(const Student&) 拷贝构造" << endl;
    }

    Student& operator=(const Student& s)
    {
        cout << "  Student::operator=(const Student&) 赋值" << endl;
        if (this != &s)
        {
            Person::operator=(s);   // 隐藏了基类的 operator=，必须显式调用
            _num = s._num;
        }
        return *this;
    }

    ~Student()
    {
        cout << "  ~Student() 析构" << endl;
    }

protected:
    int _num;
};
}

void test_default_members()
{
    cout << "--- 构造 s1 ---" << endl;
    dm::Student s1("jack", 18);
    cout << "--- 拷贝构造 s2(s1) ---" << endl;
    dm::Student s2(s1);
    cout << "--- 构造 s3 ---" << endl;
    dm::Student s3("rose", 17);
    cout << "--- 赋值 s1 = s3 ---" << endl;
    s1 = s3;
    cout << "--- 离开作用域：析构顺序 = 派生类先、基类后 ---" << endl;
}

// ============================================================================
// 第五节附：不能被继承的类
// ============================================================================
namespace fin
{
// C++11：final 修饰的类不能被继承
class Base final
{
public:
    void func5() { cout << "Base::func5（final 类，禁止被继承）" << endl; }
};

// class Derive : public Base {};   // 取消注释即编译报错：Base 是 final 类

// C++98 的方法：基类构造函数私有化——派生类的构造必须调用基类构造，
// 私有之后派生类看不见、没法调，自然就"无法实例化出对象"
}

void test_final()
{
    fin::Base b;
    b.func5();
}

// ============================================================================
// 第六节：友元与静态成员
// ============================================================================
namespace fs
{
class Student;   // 前置声明

class Person
{
public:
    friend void Display(const Person& p, const Student& s);
protected:
    string _name = "peter";
};

class Student : public Person
{
public:
    friend void Display(const Person& p, const Student& s);
protected:
    int _stuNum = 20260001;
};

void Display(const Person& p, const Student& s)
{
    cout << "Display: " << p._name << " / " << s._stuNum << endl;
}

// 静态成员：整个继承体系只有一份
class P2
{
public:
    string _name = "p";
    static int _count;
};
int P2::_count = 0;

class S2 : public P2
{
protected:
    int _stuNum = 1;
};
}

void test_friend_static()
{
    fs::Person p;
    fs::Student s;
    fs::Display(p, s);   // 不把 Student 也设为友元，这里访问 s._stuNum 会编译报错

    fs::P2 p2;
    fs::S2 s2;
    cout << "非静态成员 _name 的地址不同: " << (void*)&p2._name << " vs " << (void*)&s2._name
         << "（派生类各自继承一份）" << endl;
    cout << "静态成员 _count 的地址: " << (void*)&p2._count << " vs " << (void*)&s2._count
         << "（两个地址一模一样：全体系共用一份）" << endl;
    cout << "两个类域都能访问: " << fs::P2::_count << " / " << fs::S2::_count << endl;
}

// ============================================================================
// 第七节：多继承、菱形继承与虚继承
// ============================================================================
namespace dia
{
// ---- 非虚继承版（菱形：数据冗余 + 二义性）----
class Person
{
public:
    string _name = "person";
};

class Student : public Person
{
protected:
    int _num = 1;
};

class Teacher : public Person
{
protected:
    int _id = 2;
};

class Assistant : public Student, public Teacher
{
protected:
    string _majorCourse = "C++";
};

// ---- 虚继承版（virtual public：只保留一份）----
class PersonV
{
public:
    PersonV(const char* name = "personV") : _name(name) {}
    string _name;
};

class StudentV : virtual public PersonV
{
public:
    StudentV(const char* name, int num) : PersonV(name), _num(num) {}
protected:
    int _num;
};

class TeacherV : virtual public PersonV
{
public:
    TeacherV(const char* name, int id) : PersonV(name), _id(id) {}
protected:
    int _id;
};

class AssistantV : public StudentV, public TeacherV
{
public:
    // 虚基类由"最派生类"初始化：三个名字里只有 name3 生效
    AssistantV(const char* name1, const char* name2, const char* name3)
        : PersonV(name3)
        , StudentV(name1, 1)
        , TeacherV(name2, 2)
    {}
protected:
    string _majorCourse;
};

// ---- 多继承指针偏移 ----
class Base1 { public: int _b1 = 1; };
class Base2 { public: int _b2 = 2; };
class Derive : public Base1, public Base2 { public: int _d = 3; };
}

void test_diamond()
{
    // 1. 非虚菱形：两份 Person
    dia::Assistant a;
    a.Student::_name = "xxx（Student 那份）";
    a.Teacher::_name = "yyy（Teacher 那份）";
    cout << "非虚菱形：Student::_name = " << a.Student::_name << endl;
    cout << "非虚菱形：Teacher::_name = " << a.Teacher::_name << "（两份数据，冗余）" << endl;
    // a._name = "peter";   // 编译报错 C2385：访问不明确（二义性）
    cout << "sizeof(非虚 Assistant) = " << sizeof(dia::Assistant) << " 字节" << endl;

    // 2. 虚菱形：一份 Person（虚基类）
    dia::AssistantV av("张三", "李四", "王五");
    cout << "虚菱形：av._name = " << av._name
         << "（最派生类 AssistantV 执行的 PersonV(\"王五\") 生效）" << endl;
    cout << "sizeof(虚 AssistantV) = " << sizeof(dia::AssistantV) << " 字节" << endl;

    // 3. 多继承指针偏移
    dia::Derive d;
    dia::Base1* p1 = &d;
    dia::Base2* p2 = &d;
    dia::Derive* p3 = &d;
    cout << "p1 == p3 ? " << (p1 == (dia::Base1*)p3)
         << "，p1 != p2 ? " << ((void*)p1 != (void*)p2) << endl;
    cout << "字节偏移：p1->" << ((char*)p1 - (char*)p3)
         << "，p2->" << ((char*)p2 - (char*)p3) << "（先继承的在前）" << endl;
}

// ============================================================================
// 第八节：继承与组合
// ============================================================================
namespace comp
{
// Tire 和 Car 是 has-a：Car 有四个 Tire
class Tire
{
protected:
    string _brand = "Michelin";
    size_t _size = 17;
};

class Car
{
protected:
    string _colour = "白色";
    string _num = "陕ABIT00";
    Tire _t1, _t2, _t3, _t4;   // 组合：把轮胎"装"进来
};

// BMW 和 Car 是 is-a：宝马是一种车
class BMW : public Car
{
public:
    void Drive() { cout << "好开-操控" << endl; }
};

// stack 与 vector 两种关系都符合时，优先组合（耦合更低）
template <class T>
class stack2
{
public:
    void push(const T& x) { _v.push_back(x); }
    void pop() { _v.pop_back(); }
    T& top() { return _v.back(); }
    bool empty() { return _v.empty(); }
private:
    vector<T> _v;   // has-a：有一个 vector
};
}

void test_compose()
{
    comp::BMW bmw;
    bmw.Drive();

    comp::stack2<int> st;
    st.push(7);
    st.push(8);
    cout << "组合版 stack2: " << st.top() << "（is-a / has-a 都符合时，优先组合）" << endl;
}

// ============================================================================
// main：按文章节次顺序跑全部实验
// ============================================================================
int main()
{
    cout << "========== 第一节 基础继承与访问权限 ==========" << endl;
    test_inherit_basic();

    cout << endl << "========== 第二节 继承类模板 ==========" << endl;
    test_inherit_template();

    cout << endl << "========== 第三节 赋值兼容与对象切片 ==========" << endl;
    test_convert_slice();

    cout << endl << "========== 第四节 作用域与隐藏 ==========" << endl;
    test_hiding();

    cout << endl << "========== 第五节 默认成员函数全链路 ==========" << endl;
    test_default_members();

    cout << endl << "========== 第五节附 不能被继承的类 ==========" << endl;
    test_final();

    cout << endl << "========== 第六节 友元与静态成员 ==========" << endl;
    test_friend_static();

    cout << endl << "========== 第七节 多继承、菱形继承与虚继承 ==========" << endl;
    test_diamond();

    cout << endl << "========== 第八节 继承与组合 ==========" << endl;
    test_compose();

    return 0;
}

//#include"string.h"
//
//// 失败计数:每有一个测试失败就 +1,main 最后打印总数
//static int g_fail = 0;
//
//// Check:条件 cond 为真打印 PASS,否则打印 FAIL 并计数
//// 每个测试独立运行,失败不会中断后续测试
//void Check(bool cond, const char* name)
//{
//	if (cond)
//	{
//		cout << "[PASS] " << name << endl;
//	}
//	else
//	{
//		cout << "[FAIL] " << name << endl;
//		++g_fail;
//	}
//}
//
//// 测试点:三种构造(默认 / 带参 / 拷贝)
//// 预期:默认构造为空串、size 为 0;带参构造 size 与内容正确;拷贝构造深拷贝
//void test_ctor()
//{
//	cout << "---- 构造函数 ----" << endl;
//
//	BaiYang::string s1;
//	Check(s1.size() == 0, "默认构造: size == 0");
//	Check(strcmp(s1.c_str(), "") == 0, "默认构造: 内容为空串");
//
//	BaiYang::string s2("hello world");
//	Check(s2.size() == 11, "带参构造: size == 11");
//	Check(strcmp(s2.c_str(), "hello world") == 0, "带参构造: 内容正确");
//
//	BaiYang::string s3(s2);
//	Check(s3.size() == s2.size(), "拷贝构造: size 一致");
//	Check(strcmp(s3.c_str(), s2.c_str()) == 0, "拷贝构造: 内容一致");
//}
//
//// 测试点:operator=(普通赋值、自赋值)
//// 预期:赋值后内容一致;自赋值不崩溃且内容不变
//void test_assign()
//{
//	cout << "---- operator= ----" << endl;
//
//	BaiYang::string s1("hello");
//	BaiYang::string s2("world");
//	s2 = s1;
//	Check(strcmp(s2.c_str(), "hello") == 0, "赋值: 内容正确");
//	Check(s2.size() == 5, "赋值: size 正确");
//
//	s1 = s1; // 自赋值:应安全无副作用
//	Check(strcmp(s1.c_str(), "hello") == 0, "自赋值: 内容不变");
//}
//
//// 测试点:reserve 扩容、resize 变大(默认填 \0 / 指定填 ch)、resize 缩小
//// 预期:
////   reserve 只扩不减,原内容与 size 不变
////   resize 变大:新位置填 ch(默认 \0),末尾补 \0,size 更新
////   resize 缩小:在 n 处截断并补 \0,size 更新
//void test_reserve_resize()
//{
//	cout << "---- reserve / resize ----" << endl;
//
//	BaiYang::string s1("hello");
//	size_t oldCap = s1.capacity();
//	s1.reserve(oldCap + 10);
//	Check(s1.capacity() >= oldCap + 10, "reserve: 容量扩大");
//	Check(strcmp(s1.c_str(), "hello") == 0, "reserve: 内容不变");
//	Check(s1.size() == 5, "reserve: size 不变");
//
//	BaiYang::string s2("abc");
//	s2.resize(6); // 变大,默认填 '\0'
//	Check(s2.size() == 6, "resize 变大: size == 6");
//	Check(s2.c_str()[3] == '\0' && s2.c_str()[5] == '\0', "resize 变大: 新位置填 \\0");
//
//	BaiYang::string s3("ab");
//	s3.resize(5, 'x'); // 变大,指定填 'x'
//	Check(strcmp(s3.c_str(), "abxxx") == 0, "resize 变大: 填充 ch 正确");
//	Check(s3.size() == 5, "resize 变大: size == 5");
//
//	BaiYang::string s4("hello world");
//	s4.resize(5); // 缩小
//	Check(s4.size() == 5, "resize 缩小: size == 5");
//	Check(strcmp(s4.c_str(), "hello") == 0, "resize 缩小: n 处补 \\0");
//}
//
//// 测试点:push_back 从空串开始连续插入(重点:容量从 0 开始的扩容路径)
//// 预期:内容为 "abcdefghij",size == 10,不越界
//void test_push_back()
//{
//	cout << "---- push_back ----" << endl;
//
//	BaiYang::string s1;
//	for (char ch = 'a'; ch <= 'j'; ++ch)
//	{
//		s1.push_back(ch);
//	}
//	Check(s1.size() == 10, "push_back: size == 10");
//	Check(strcmp(s1.c_str(), "abcdefghij") == 0, "push_back: 内容正确");
//}
//
//// 测试点:append 向空串追加、向非空串多次追加
//// 预期:每次追加到原串末尾,最终内容完整
//void test_append()
//{
//	cout << "---- append ----" << endl;
//
//	BaiYang::string s1;
//	s1.append("hello");
//	Check(strcmp(s1.c_str(), "hello") == 0, "append 空串: 内容正确");
//
//	s1.append(" world");
//	s1.append("!");
//	Check(strcmp(s1.c_str(), "hello world!") == 0, "append 多次追加: 内容完整");
//	Check(s1.size() == 12, "append: size == 12");
//
//	BaiYang::string s2("abc");
//	s2.append("def");
//	Check(strcmp(s2.c_str(), "abcdef") == 0, "append 非空串: 追加位置正确");
//}
//
//// 测试点:operator+= 两种重载(char / const char*)及链式追加
//// 预期:依次追加,内容完整
//void test_plus_eq()
//{
//	cout << "---- operator+= ----" << endl;
//
//	BaiYang::string s1("hi");
//	s1 += '!';
//	s1 += " there";
//	Check(strcmp(s1.c_str(), "hi! there") == 0, "+= : 内容正确");
//	Check(s1.size() == 9, "+= : size == 9");
//}
//
//// ---- insert 尚未实现,以下测试暂时注释,实现后取消注释即可 ----
//// 预期:char 版与 const char* 版都支持头插 / 中插 / 尾插,size 同步更新
///*
//void test_insert()
//{
//	cout << "---- insert ----" << endl;
//
//	BaiYang::string s1("hello");
//	s1.insert(0, 'A'); // 头插 -> "Ahello"
//	s1.insert(6, 'B'); // 尾插 -> "AhelloB"
//	s1.insert(3, 'C'); // 中插 -> "AheClloB"
//	Check(strcmp(s1.c_str(), "AheClloB") == 0, "insert(char): 头/中/尾");
//	Check(s1.size() == 8, "insert(char): size 同步");
//
//	BaiYang::string s2("ab");
//	s2.insert(0, "xx"); // 头插 -> "xxab"
//	s2.insert(4, "yy"); // 尾插 -> "xxabyy"
//	s2.insert(2, "zz"); // 中插 -> "xxzzabyy"
//	Check(strcmp(s2.c_str(), "xxzzabyy") == 0, "insert(const char*): 头/中/尾");
//	Check(s2.size() == 8, "insert(const char*): size 同步");
//}
//*/
//
//// ---- 以下接口尚未实现,实现后按下面预期补写对应测试 ----
//// 待实现清单与预期:
////   erase(pos, len) : 删除 [pos, pos+len) 区间,pos 越界应 assert
////   find(ch, pos)   : 从 pos 起找字符,返回首次出现下标,找不到返回 npos
////   find(str, pos)  : 从 pos 起找子串,同上
////   substr(pos, len): 返回子串,len 超过剩余长度时截断到末尾
////   swap(other)     : 交换两个对象的内容 / size / capacity
//
//int main()
//{
//	test_ctor();
//	//test_assign();
//	//test_reserve_resize();
//	//test_push_back();
//	//test_append();
//	//test_plus_eq();
//	// test_insert(); // insert 实现后取消注释
//
//	cout << endl << "测试结束,失败数: " << g_fail << endl;
//	return g_fail;
//}




#include<iostream>
#include<string>
using namespace std;

#include"string.h"

void func(const bit::string& s) {
	for (auto& ch : s) {
		cout << ch << " ";
	}
	cout << endl;

	// 迭代器
	//bit::string::const_iterator it = s.begin();
	auto it = s.begin();
	while (it != s.end()) {
		// *it = 'x';
		cout << *it << '%';
		++it;
	}
	cout << endl;
}

void teststring1(){
	std::string s3;
	cout << s3.c_str() << endl;

	bit::string s2;
	cout << s2.c_str() << endl;

	bit::string s1("hello world");
	cout << s1.c_str() << endl;

	// 下标+[]  最常用
	for (size_t i = 0; i < s1.size(); i++) {
		s1[i]++;
		cout << s1[i] << '|';
	}
	cout << endl;

	for (auto& ch : s1) {
		ch--;
	}
	cout << endl;

	// 迭代器
	bit::string::iterator it = s1.begin();
	while (it != s1.end()) {
		cout << *it << '%';
		++it;
	}
	cout << endl;
}

void teststring2() {
	/*std::string s1("hello");
	s1.append("xxxxxxxxxxxx");

	cout << s1.c_str() << endl;

	s1 += ' ';
	s1 += "yyyyyyyyyyyyyy";
	cout << s1.c_str() << endl;*/

	string s1("string.cpp");
	string s2("aaaa");
	const char* p1 = "bb";
	//cout << (s1 < s2) << endl;
	//cout << (s2 < p1) << endl;
	//cout << (p1 < s2) << endl;

	cout << s1 + p1 << endl;
	cout << p1 + s1 << endl;
	cout << s1 + p1 + s2 + p1 << endl;
}

void teststring3() {
	bit::string s1("hello world");
	s1.insert(5, ' ');
	s1.insert(0, 'x');
	cout << s1.c_str() << endl;

	bit::string s2;
	s2.insert(0, 'x');
	cout << s2.c_str() << endl;

	bit::string s3("hello world");
	s3.insert(5, "xxx");
	s3.insert(0, "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy");
	cout << s3.c_str() << endl;

	bit::string s4("hello worldxxxxxxxxxxxx");
	//s4.erase(11, 20);
	s4.erase(11);
	cout << s4.c_str() << endl;

	s4.erase(6, 3);
	cout << s4.c_str() << endl;

	bit::string s5("abcabexxxx");
	cout << s5.find("abe") << endl;
	cout << s5.find("abx") << endl;

	bit::string ret = s5.substr(3, 4);
	//cout << &ret << endl;
	cout << ret.c_str() << endl;

	bit::string s6(s5);
	cout << s5.c_str() << endl;
	cout << s6.c_str() << endl;

	s6 = s3;
	cout << s6.c_str() << endl;
	cout << s3.c_str() << endl;

	s6 = s6;
	cout << s6.c_str() << endl;
}

//void teststring4() {
//	bit::string s1("hello world");
//	bit::string s2("xxxx");
//	const char* str = "yyyy";
//	cout << s1 + s2 << endl;
//	cout << s1 + str << endl;
//	cout << str + s1 << endl;
//
//	swap(s1, s2);
//	s1.swap(s2);
//
//	bit::string s3;
//	//cin >> s3;
//	getline(cin, s3);
//	cout << s3 << endl;
//}

// ============================================================
// 新追加:PASS/FAIL 自动测试套件(覆盖全部接口)
// 每个测试注明测试点与预期;失败打印 [FAIL] 并计数,不中断
// ============================================================

// 失败计数:每有一个测试失败就 +1,main 最后打印总数
static int g_fail = 0;

// Check:条件成立打印 PASS,否则打印 FAIL 并计数
void Check(bool cond, const char* name)
{
	if (cond)
	{
		cout << "[PASS] " << name << endl;
	}
	else
	{
		cout << "[FAIL] " << name << endl;
		++g_fail;
	}
}

// 测试点:三种构造(默认 / 带参 / 拷贝)
// 预期:默认构造为空串 size==0;带参构造内容与 size 正确;拷贝构造内容一致
void test_ctor()
{
	cout << "---- 构造 ----" << endl;

	bit::string s1;                          // 默认构造
	Check(s1.size() == 0, "默认构造 size==0");
	Check(strcmp(s1.c_str(), "") == 0, "默认构造内容为空");

	bit::string s2("hello world");           // 带参构造
	Check(s2.size() == 11, "带参构造 size==11");
	Check(strcmp(s2.c_str(), "hello world") == 0, "带参构造内容正确");

	bit::string s3(s2);                      // 拷贝构造
	Check(strcmp(s3.c_str(), s2.c_str()) == 0, "拷贝构造内容一致");
	Check(s3.size() == s2.size(), "拷贝构造 size 一致");
}

// 测试点:operator=(普通赋值、自赋值)
// 预期:赋值后内容一致;自赋值不崩溃且内容不变
void test_assign()
{
	cout << "---- 赋值 ----" << endl;

	bit::string s1("hello");
	bit::string s2("world");
	s2 = s1;
	Check(strcmp(s2.c_str(), "hello") == 0, "赋值内容正确");
	Check(s2.size() == 5, "赋值 size 正确");

	s1 = s1;                                 // 自赋值:应安全无副作用
	Check(strcmp(s1.c_str(), "hello") == 0, "自赋值内容不变");
}

// 测试点:reserve 扩容、resize 变大(填 ch / 默认 \0)、resize 缩小
// 预期:reserve 只扩不减且内容不变;resize 变大填充正确并补 \0;缩小正确截断
void test_reserve_resize()
{
	cout << "---- reserve/resize ----" << endl;

	bit::string s1("hello");
	size_t oldCap = s1.capacity();
	s1.reserve(oldCap + 10);
	Check(s1.capacity() >= oldCap + 10, "reserve 扩容成功");
	Check(strcmp(s1.c_str(), "hello") == 0, "reserve 内容不变");
	Check(s1.size() == 5, "reserve size 不变");

	bit::string s2("ab");
	s2.resize(5, 'x');                       // 变大,指定填 'x'
	Check(s2.size() == 5, "resize 变大 size==5");
	Check(strcmp(s2.c_str(), "abxxx") == 0, "resize 变大填充 ch 正确");

	bit::string s3("abc");
	s3.resize(6);                            // 变大,默认填 '\0'
	Check(s3.size() == 6, "resize 默认填充 size==6");
	Check(s3.c_str()[3] == '\0' && s3.c_str()[5] == '\0', "resize 默认填充 \\0");

	bit::string s4("hello world");
	s4.resize(5);                            // 缩小
	Check(s4.size() == 5, "resize 缩小 size==5");
	Check(strcmp(s4.c_str(), "hello") == 0, "resize 缩小正确截断");
}

// 测试点:push_back 从空串开始连续插入
// 预期:重点验证容量从 0 起的倍增扩容路径,内容 "abcdefghij",size==10
void test_push_back()
{
	cout << "---- push_back ----" << endl;

	bit::string s1;
	for (char ch = 'a'; ch <= 'j'; ++ch)
	{
		s1.push_back(ch);
	}
	Check(s1.size() == 10, "push_back size==10");
	Check(strcmp(s1.c_str(), "abcdefghij") == 0, "push_back 内容正确");
}

// 测试点:append 空串追加、非空串多次追加
// 预期:每次都追加在原串末尾,最终内容完整
void test_append()
{
	cout << "---- append ----" << endl;

	bit::string s1;
	s1.append("hello");
	Check(strcmp(s1.c_str(), "hello") == 0, "append 空串追加正确");

	s1.append(" world");
	s1.append("!");
	Check(strcmp(s1.c_str(), "hello world!") == 0, "append 多次追加内容完整");
	Check(s1.size() == 12, "append size==12");
}

// 测试点:operator+= 两种重载(char / const char*)
// 预期:依次追加,内容与 size 正确
void test_plus_eq()
{
	cout << "---- operator+= ----" << endl;

	bit::string s1("hi");
	s1 += '!';
	s1 += " there";
	Check(strcmp(s1.c_str(), "hi! there") == 0, "+= 内容正确");
	Check(s1.size() == 9, "+= size==9");
}

// 测试点:insert 头插 / 中插 / 尾插(char 与 const char* 两种)
// 预期:插入位置正确,size 同步更新
void test_insert()
{
	cout << "---- insert ----" << endl;

	bit::string s1("hello");
	s1.insert(0, 'A');                       // 头插 -> "Ahello"
	s1.insert(6, 'B');                       // 尾插 -> "AhelloB"
	s1.insert(3, 'C');                       // 中插 -> "AheClloB"
	Check(strcmp(s1.c_str(), "AheClloB") == 0, "insert(char) 头/中/尾");
	Check(s1.size() == 8, "insert(char) size 同步");

	bit::string s2("ab");
	s2.insert(0, "xx");                      // 头插 -> "xxab"
	s2.insert(4, "yy");                      // 尾插 -> "xxabyy"
	s2.insert(2, "zz");                      // 中插 -> "xxzzabyy"
	Check(strcmp(s2.c_str(), "xxzzabyy") == 0, "insert(const char*) 头/中/尾");
	Check(s2.size() == 8, "insert(const char*) size 同步");
}

// 测试点:erase 部分删除 / 删到末尾 / len 超过剩余长度
// 预期:删除区间正确,末尾补 \0,size 同步
void test_erase()
{
	cout << "---- erase ----" << endl;

	bit::string s1("hello world");
	s1.erase(5, 6);                          // 删 " world" -> "hello"
	Check(strcmp(s1.c_str(), "hello") == 0, "erase 部分删除");
	Check(s1.size() == 5, "erase 部分删除 size");

	bit::string s2("hello world");
	s2.erase(5);                             // 从 5 删到末尾 -> "hello"
	Check(strcmp(s2.c_str(), "hello") == 0, "erase 删到末尾");

	bit::string s3("hello");
	s3.erase(2, 100);                        // len 超过剩余长度,删到末尾 -> "he"
	Check(strcmp(s3.c_str(), "he") == 0, "erase len 超长截断");
}

// 测试点:find 字符版 / 子串版,含指定起点与找不到返回 npos
// 预期:返回首次出现的下标;找不到返回 string::npos
void test_find()
{
	cout << "---- find ----" << endl;

	bit::string s1("abcabexxxx");
	Check(s1.find('e') == 5, "find(char) 找到");  // "abcabexxxx" 中 e 在下标 5
	Check(s1.find('z') == bit::string::npos, "find(char) 找不到返回 npos");
	Check(s1.find("abe") == 3, "find(str) 找到");
	Check(s1.find("abx") == bit::string::npos, "find(str) 找不到返回 npos");
	Check(s1.find('b', 3) == 4, "find(char,pos) 从指定位置找");
}

// 测试点:substr 正常截取 / len 超过剩余长度
// 预期:返回正确子串;len 超长时截断到末尾
void test_substr()
{
	cout << "---- substr ----" << endl;

	bit::string s1("abcabexxxx");
	bit::string ret = s1.substr(3, 4);       // "abex"
	Check(strcmp(ret.c_str(), "abex") == 0, "substr 正常截取");

	bit::string ret2 = s1.substr(3, 100);    // len 超长 -> "abexxxx"
	Check(strcmp(ret2.c_str(), "abexxxx") == 0, "substr len 超长截断");
}

// 测试点:swap 成员交换
// 预期:两个对象的内容 / size / capacity 全部互换
void test_swap()
{
	cout << "---- swap ----" << endl;

	bit::string s1("aaa");
	bit::string s2("bbbbbb");
	s1.swap(s2);
	Check(strcmp(s1.c_str(), "bbbbbb") == 0, "swap s1 内容");
	Check(strcmp(s2.c_str(), "aaa") == 0, "swap s2 内容");
	Check(s1.size() == 6 && s2.size() == 3, "swap size 同步");
}

// 测试点:operator[] 读写、非 const / const 迭代器遍历
// 预期:下标可读可写;begin/end 遍历次数等于 size
void test_iterator()
{
	cout << "---- operator[]/迭代器 ----" << endl;

	bit::string s1("abc");
	s1[0] = 'x';                             // 下标写
	Check(s1[0] == 'x', "operator[] 读写");
	Check(strcmp(s1.c_str(), "xbc") == 0, "operator[] 写后内容");

	size_t cnt = 0;
	for (bit::string::iterator it = s1.begin(); it != s1.end(); ++it)
	{
		++cnt;
	}
	Check(cnt == 3, "非 const 迭代器遍历次数");

	const bit::string s2("abc");
	cnt = 0;
	for (bit::string::const_iterator it = s2.begin(); it != s2.end(); ++it)
	{
		++cnt;
	}
	Check(cnt == 3, "const 迭代器遍历次数");
}

// 依次运行你的测试 + 自动测试,打印失败总数;全部通过返回 0
int main() {
	teststring1();
	teststring2();
	teststring3();

	test_ctor();
	test_assign();
	test_reserve_resize();
	test_push_back();
	test_append();
	test_plus_eq();
	test_insert();
	test_erase();
	test_find();
	test_substr();
	test_swap();
	test_iterator();

	cout << endl << "自动测试结束,失败数: " << g_fail << endl;
	return g_fail;
}


#include<iostream>
#include<vector>

using namespace std;
//代码一：验证几个构造
//int main()
//{
//	vector<int> v1;                        // (1) 无参构造
//	vector<int> v2(4, 100);                // (2) 4个100
//	vector<int> v3(v2.begin(), v2.end());  // (3) 迭代器区间构造
//	vector<int> v4(v3);                    // (4) 拷贝构造
//	vector<int> v5{ 1, 2, 3, 4, 5 };       // (5) C++11 初始化列表
//
//	cout << "v2表内数据：";
//	for (auto e : v2)
//	{
//		cout << e << " ";
//	}
//	cout << endl;
//
//	cout << "v3表内数据：";
//	for (auto e : v3)
//	{
//		cout << e << " ";
//	}
//	cout << endl;
//
//	cout << "v4表内数据：";
//	for (auto e : v4)
//	{
//		cout << e << " ";
//	}
//	cout << endl;
//	cout << "v5表内数据：";
//    vector<int>::iterator it = v5.begin();
//	while (it != v5.end())
//	{
//		cout << *it << " ";
//		++it;
//	}
//	cout << endl;
//
//
//	return 0;
//}


//代码二：三种遍历：
//int main()
//{
//	vector<int>v{ 1,2,3,4,5 };
//
//	//1:[]遍历：
//	cout << "[]遍历:";
//	for (int i = 0; i < v.size(); i++)
//		cout << v[i] << " ";
//	cout << endl;
//	//迭代器
//	cout << "迭代器:";
//	for (vector<int>::iterator it = v.begin(); it < v.end(); it++)
//	{
//		cout << *it << " ";
//	}
//	cout << endl;
//	//auto
//	cout << "auto:";
//	for (auto e : v)
//	{
//		cout << e << " ";
//	}
//	return 0;
//}
void print(vector<int >v)
{
for (auto e : v)
			{
				cout << e << " ";
			}
}
//about space
//int main()
//{
//	vector<int>v;
//	cout << "size=" << v.size() << " capacity=" << v.capacity() << endl;  // 0 0
//
//	v.reserve(100);
//	cout << "reserve(100)后: size=" << v.size() << " capacity=" << v.capacity() << endl;  // 0 100
//
//	v.resize(5, 1);
//	cout << "resize(5,1)后: size=" << v.size() << " capacity=" << v.capacity() << endl;   // 5 100
//
//	v.resize(2);
//	cout << "resize(2)后: size=" << v.size() << " capacity=" << v.capacity() << endl;     // 2 100
//
//	return 0;
//}

//开空间测试
//int main()
//{
//	vector<int>v;
//
//	size_t sz = v.capacity();
//
//	for (size_t i = 0; i < 100; i++)
//	{
//		v.push_back(1);
//		
//		if (sz != v.capacity())
//		{
//			sz = v.capacity();
//			cout << "capacity changed:" << sz << endl;
//		}
//	}
//
//	return 0;
//}


//int main()
//{
//    vector<int> v{ 1, 2, 3, 4, 5 };
//
//    v.push_back(6);            // 尾插
//    v.pop_back();              // 尾删
//
//    // 查找：用库函数 find，返回迭代器
//    auto it = find(v.begin(), v.end(), 3);
//    if (it != v.end())
//        cout << "find(3) 在下标 " << (it - v.begin()) << " 处" << endl;
//
//    v.insert(v.begin(), 0);    // 头部插入 0
//    v.erase(v.begin() + 1);    // 删除下标1的元素
//
//    v[0] = 10;                 // operator[] 支持修改
//    cout << "at(0)=" << v.at(0) << " front=" << v.front() << " back=" << v.back() << endl;
//    return 0;
//}

//int main()
//{
//	vector<int>v;
//
//	v = { 1,2,3,4,5 };
//	v.push_back(6);
//	v.pop_back();
//	print(v);
//
//	auto it = find(v.begin(), v.end(),3);
//	if (it != v.end())
//	{
//		cout<< "find(3) 在下标 " << (it - v.begin()) << " 处" << endl;
//	}
//
//	v.insert(v.begin(), 11);
//	v.erase(v.end());//end()是坑
//
//	print(v);
//	cout << endl;
//
//	v[0] = 12;
//	print(v);
//	return 0;
//}
//


//迭代器失效
//int main()
//{
//
//	/*vector<int>v{ 1,2,3,4,5,6,7 };
//
//	auto it = v.begin();
//
//	v.reserve(100);
//	while (it != v.end())
//	{
//		cout << *it << " ";
//		++it;
//	}
//	cout << endl;
//	return 0;*/
//	vector<int> v{ 1, 2, 3, 4 };
//	auto it = v.begin();
//	while (it != v.end())
//	{
//		if (*it % 2 == 0)
//			it=v.erase(it);// it 已经失效
//		else
//		++it;              // 对失效迭代器 ++
//	}
//	print(v);
//
//}

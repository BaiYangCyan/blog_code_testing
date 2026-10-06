#include<iostream>
#include"vector.h"
#include<vector>
using namespace std;
void test1()
{
	bit::vector<int>v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
	v1.push_back(5);

	for (auto& e : v1)
	{
		cout << e << " ";
	}
	cout << endl;
	v1.pop_back();
	v1.pop_back();
	bit::vector<int>v2(v1);
	for (auto& e : v2)
	{
		cout << e << " ";
	}
	cout << endl;
	v2.reserve(100);
	cout << v2.size() << " " << v2.capacity() << endl;

	v2.resize(10, 1);
	for (auto& e : v2)
	{
		cout << e << " ";
	}
	cout << endl;
	cout << v2.size() << " " << v2.capacity() << endl;

	v2.resize(13);
	for (auto& e : v2)
	{
		cout << e << " ";
	}
	cout << endl;
	cout << v2.size() << " " << v2.capacity() << endl;
	v2.resize(3);
	for (auto& e : v2)
	{
		cout << e << " ";
	}
	cout << endl;
	cout << v2.size() << " " << v2.capacity() << endl;
}


void test2()
{
	bit::vector<int>v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
	

	v1.insert(v1.begin(), 1);

	v1.insert(v1.begin(), 1);
	for (auto& e : v1)
	{
		cout << e << " ";
	
	}
	cout << endl;

	v1.erase(v1.begin());
	for (auto& e : v1)
	{
		cout << e << " ";

	}
	cout << endl;
}
int main()
{
	test2();
	return 0;
}
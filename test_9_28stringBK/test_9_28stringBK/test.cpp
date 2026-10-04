#include<iostream>
#include<string>
using namespace std;
int main()
{
	//基础接口测试
	/*string s("hello bit");

	cout << "size:" << s.size() << endl;
	cout << "length:" << s.length() << endl;
	cout << "capacity:" << s.capacity() << endl;
	cout << "empty:" << s.empty() << endl;*/


	//resize接口测试
	//string s("hello");
	//cout << s.size() << " " << s.capacity() << endl;

	//s.resize(8, 'a');//增多末尾补‘a'
	//cout << s <<"  "<<"size=" << s.size() << endl;

	//s.resize(3);
	//cout << s <<" " << "size=" << s.size() << " " << "capacity:" << s.capacity() << endl;

	////capacity==15容量不变。但size=3
	//s.resize(11);
	//cout << s << " " << "size=" << s.size() << " " << "capacity:" << s.capacity() << endl;
	//未初始化的补\0;



	//reserve接口测试
	string s("hello");
	cout << "capacity:" << s.capacity() << endl;

	s.reserve(100);
	cout << "capacity:" << s.capacity() << endl;
	cout << s << endl;
	cout << s.size() << endl;

	s.reserve(3);
	cout << "capacity:" << s.capacity() << endl;



	//clear与empty
	/*string s("123456");
	cout << s.capacity() << endl;
	s.clear();
	cout << s.capacity() << endl;
	cout << s.size() << endl;
	cout << s.empty() << endl;*/




	//shrink_to_fit接口测试
	/*string s("hello world");
	
	s.reserve(100);
	cout << s.capacity() << endl;
	s.shrink_to_fit();
	cout << s.capacity() << endl;*/




	//string扩容测试：
	/*string s("a");

	int old_capacity = 0;
	for (int i = 0; i < 200; i++)
	{
		s.push_back('b');
		if (old_capacity != s.capacity())
		{
			cout << "capacity:" << s.capacity() << endl;
		}

     	old_capacity = s.capacity();

	}*/




	 //+=接口测试
	/*string s("hello");

	s += " world";
	cout << s << endl;

	s += '!';
	cout << s << endl;

	s += "C++";
	cout << s << endl;*/



	//insert接口测试

	//string s("my name is pengyuyan");
	//s.insert(0, "jj");//jjmy name is pengyuyan
	//cout << s << endl;
	//s.insert(2, 2, 'x');//jjxxmy name is pengyuyan
	//cout << s << endl;

    

    //接口assign测试

	/*string s("Hello wrold");
	s.assign("Callo wrold");
	cout << s << endl;
	s.assign(5,'c');
	cout << s << endl;*/





    //replace接口测试
    /*string s("hello world");

	s.replace(6, 5, "C++");
	cout << s << endl;

	s.replace(0, 1, 2, 'A');
	cout << s << endl;*/

    

    //erase接口测试

    /*string s("123456789");

	s.erase(3, 3);
	cout << s << endl;
	s.erase(s.begin() + 1);
	cout << s << endl;*/





    //pop_back接口测试
    /*string s("hello world");
	s.pop_back();
	cout << s << endl;*/

     
    //find rfind 接口测试
    /*string s("hello world hello");

	size_t pos = s.find("world");
	if (pos != string::npos)
	{
		cout << "\"world\"在下标" << pos << "处" << endl;
	}
	cout << "find(\"xyz\") = " << s.find("xyz") << endl;
	cout << "rfind(\"hello\") = " << s.rfind("hello") << endl;*/


	//string s("aa bb cc aa");
	//size_t pos = s.find("aa");
	//while (pos != string::npos)
	//{
	//	cout << "找到 aa，位置 " << pos << endl;
	//	pos = s.find("aa", pos + 1);   // 从下一个位置继续
	//}

    //substr接口测试
       /*string s("hello world");
	   cout << s.substr(6, 5) << endl;
	   cout << s.substr(6) << endl;*/


 


       



       /* string s1("apple"), s2("banana");
		cout << (s1 < s2) << endl;
		cout << (s1 == s2) << endl;*/


    

//string s;
//cin >> s;
//cout << s << endl;



//string line;
//
//cout << "请输入一行（含空格）：";
//getline(cin, line);
//cout << line << endl;


//int n;
//cin >> n;                  // 输入 2026 回车
//string line;
//getline(cin, line);        // 还没等你输入，就结束了！
//cout << "数字是: " << n << " | 行内容是: [" << line << "]" << endl;
    

//
//cout << sizeof(std::string) << endl;



	return 0;
}
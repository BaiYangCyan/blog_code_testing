#include"string.h"

void stringGZtest()
{
	BaiYang::string s1;
	cout << s1.c_str() << endl;

	BaiYang::string s2("hello world");
	cout << s2.c_str() << endl;

	BaiYang::string s3(s2);
	cout << s3.c_str() << endl;


}

int main()
{
	stringGZtest();

}
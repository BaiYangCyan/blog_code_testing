#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<assert.h>
#include<string.h>

// 【修正】头文件里不要 using namespace std,避免污染使用者的命名空间
// 原代码:using namespace std;
// 以下接口中的 string/cout 等一律使用 std:: 显式限定

namespace bit {
	class string
	{
	public:
		typedef char* iterator;
		// 【修正】拼写 const_iterrator -> const_iterator
		// 原代码:typedef const char* const_iterrator;
		typedef const char* const_iterator;

		iterator begin() { return _str; }
		iterator end() { return _str + _size; }

		// 【修正】拼写 beging -> begin
		// 原代码:const_iterrator beging()const { return _str; }
		//         const_iterrator end()const { return _str + _size; }
		const_iterator begin()const { return _str; }
		const_iterator end()const { return _str + _size; }

		static const size_t npos = -1;

		string();
		string(const char* str);
		string(const string& s);
		string& operator=(const string& s);
		~string();



		void resize(size_t n,char ch='\0');
		void reserve(size_t n);
		void push_back(char ch);
		void append(const char* str);
		string& operator+=(char ch);
		string& operator+=(const char* str);
		void insert(size_t pos, char ch);
		void insert(size_t pos, const char* str);
		void erase(size_t pos = 0, size_t len = string::npos);
		size_t find(char ch, size_t pos = 0)const;
		size_t find(const char* str, size_t pos = 0)const;
		string substr(size_t pos = 0, size_t len = string::npos)const;
		void swap(string&);

		const char* c_str()const { return _str; }
		size_t size()const { return _size; }
		size_t capacity()const { return _capacity; }

		char& operator[](size_t pos) {
			assert(pos < _size);
			return _str[pos];
		}

		const char& operator[](size_t pos) const {
			assert(pos < _size);
			return _str[pos];
		}


	private:
		char* _str;
		size_t _size;
		size_t _capacity;
	
	};
}
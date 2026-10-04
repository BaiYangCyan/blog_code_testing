#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<assert.h>
#include<string.h>
using namespace std;

namespace BaiYang {
	class string
	{
	public:
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

	private:
		char* _str;
		size_t _size;
		size_t _capacity;
	
	};
}
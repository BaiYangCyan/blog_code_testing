#include"string.h"

namespace BaiYang 
{
	string::string()
		:_str(new char[1])
		, _size(0)
		, _capacity(0) {
		_str[0] = '\0';
	}
	string::string(const char*str)
		:_size(strlen(str)){
		_capacity = _size;
		_str = new char[_size + 1];
		strcpy(_str, str);
	}

	string::string(const string& s)
		:_str(new char[s._capacity + 1])
		, _size(s._size)
		, _capacity(s._capacity) {
		cout << "	string::string(const string& s)" << endl;
		strcpy(_str, s._str);
	}
	string& string::operator=(const string& s)
	{
		if (this != &s)
		{
			delete[]_str;
			_str = new char[s._capacity + 1];
			strcpy(_str, s._str);
			_size = s._size;
			_capacity = s._capacity;

		}
		return *this;
	}
	string::~string()
	{
		delete[]_str;
		_str = nullptr;
		_size = _capacity = 0;
	}

	void string::reserve(size_t n)
	{
		if (n > _capacity)
		{
			char* tmp = new char[n + 1];
			strcpy(tmp, _str);
			delete[]_str;
			_str = tmp;
			_capacity = n;

		}
	}
	void string::resize(size_t n, char ch)
	{
		if (n > _size)
		{
			char* tmp = new char[n + 1];
			strcpy(tmp, _str);
			delete[]_str;
			for (int i = _size; i < _capacity; i++)
			{
				tmp[i] = '\0';
			}
			_str = tmp;
			_size = n;
			_capacity = n;
		}
		if (n < _size)
		{
			char* tmp = new char[n + 1];
			strcpy(tmp, _str);
			delete[]_str;
			_str = tmp;
			_size = n;
		}
	}

	void string::push_back(char ch)
	{
		if (_size == _capacity)
		{
			reserve(_capacity == 0 ? 4 : _capacity *= 2);
		}

		_str[_size] = ch;
		++_size;
		_str[_size] = '\0';

	}
	void string::append(const char* str)
	{
		size_t len = strlen(str);

		if ((_size + len) > _capacity)
		{
			reserve(_size+len);
		}
		strcpy(_str + len, str);
		_size += len;
	}
		
	string& string:: operator+=(char ch)
	{
		push_back(ch);
		return *this;
	}
	string& string::operator+=(const char* str)
	{
		append(str);
		return *this;

	}

	void string::insert(size_t pos, char ch)
	{
		if (_size == _capacity)
		{
			reserve(_capacity == 0 ? 4 : _capacity *= 2);
		}
		size_t end = _size;
		while (end > pos)
		{
			_str[end]
		}
	}
	void insert(size_t pos, const char* str)
	{

	}
	void erase(size_t pos = 0, size_t len = string::npos);


}
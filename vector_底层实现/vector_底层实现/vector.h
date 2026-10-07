#pragma once

#include<assert.h>
#include<string.h>


namespace bite
{
	class string
	{
	public:
		string(const char* str = "")
		{
			_str = new char[strlen(str) + 1];
			strcpy(_str, str);
		}
		string(const string& s)
			: _str(new char[strlen(s._str) + 1])
		{
			strcpy(_str, s._str);
		}
		string& operator=(const string& s)     // 传统深拷贝写法
		{
			if (this != &s)
			{
				char* tmp = new char[strlen(s._str) + 1];
				strcpy(tmp, s._str);
				delete[] _str;
				_str = tmp;
			}
			return *this;
		}
		~string()
		{
			delete[] _str;
			_str = nullptr;
		}
		const char* c_str() const { return _str; }
	private:
		char* _str;
	};
}

namespace bit
{
	template<class T>
	class vector {
	public:
		//������
		typedef T* iterator;
		typedef const T* const_iterator;

		iterator begin() { return _start; }
		iterator end() { return _finish; }

		size_t size() const{ return _finish - _start; }
		size_t capacity() const{ return _end_of_storage - _start; }
		bool empty()const { return _finish == _start; }

		T& operator[](size_t pos)
		{
			assert(pos < size());
			return _start[pos];
		}
		const T& operator[](size_t pos)const
		{
			assert(pos < size());
			return _start[pos];
		}
		//�հ׹���
		vector(){}
		//��������
		vector(const vector<T>&v)
		{
			_start = new T[v.capacity()];
			for (size_t i = 0; i < v.size(); i++)
			{
				_start[i] = v._start[i];
			}
			_finish = _start + v.size();
			_end_of_storage = _start + v.capacity();
			
		}

		//��������
		vector<T>& operator=(const vector<T>& v)
		{
			if (this != &v)
			{
				T* tmp = new T[v.capacity()];
				for (size_t i = 0; i < v.size(); i++)
				{
					tmp[i] = v._start[i];
				}
				delete[]_start;
				_start = tmp;
				_finish = _start + v.size();
				_end_of_storage = _start + v.capacity();
			}
			return *this;
		}

		//��������
		~vector()
		{
			delete[]_start;
			_finish = _end_of_storage = nullptr;
		}

		//void reserve(size_t n);
		void reserve(size_t n)
		{
			if (n > capacity())
			{
				size_t sz = size();
				T* tmp = new T[n];
				if (_start)
					memcpy(tmp, _start, sizeof(T) * sz);  // 浅拷贝！指针被原样复制
				delete[] _start;
				_start = tmp;
				_finish = _start + sz;
				_end_of_storage = _start + n;
			}
		}

		void resize(size_t n, const T& val=T());

		void push_back(const T& x);
		void pop_back();

		iterator insert(iterator pos, const T& x);
		iterator erase(iterator pos);
		
	private:
		iterator _start=nullptr;
		iterator _finish=nullptr;
		iterator _end_of_storage = nullptr;

	};
	/*template<class T>
	void vector<T>::reserve(size_t n)
	{
		size_t old_size = size();
		if (n > capacity())
		{
			T* tmp = new T[n];
			for (int i = 0; i < size(); i++)
			{
				tmp[i] = _start[i];
			}
			delete[]_start;
			_start = tmp;
			_finish = _start + old_size;
			_end_of_storage = _start+n;
		}
	}*/
	template<class T>
	void vector<T>::resize(size_t n, const T& val)
	{
		if (n > size())
		{
			if (n > capacity())
			{
				reserve(n);
			}
			for (size_t i = size(); i < n; i++)
			{
				_start[i] = val;
			}
			_finish = _start + n;
		}
		else
		{
			for (iterator it = _start + n; it != _finish; ++it)
			{
				it->~T();
			}
			_finish = _start + n;
		}
	}
	template<class T>
	void vector<T>::push_back(const T& x)
	{
		if (_finish==_end_of_storage)
		{
			size_t newcapacity =capacity() == 0 ? 4 : capacity() * 2;
			reserve(newcapacity);
		}
		*_finish = x;
		++_finish;

	}
	template<class T>
	void vector<T>::pop_back()
	{
		--_finish;
	}
	template<class T>
	typename vector<T>::iterator vector<T>::insert(iterator pos, const T& x)
	{
		size_t old_pos = pos - _start;
		assert(pos >= _start && pos <= _finish);
		if (size() == capacity())
		{
			size_t newcapacity;
			newcapacity = capacity() == 0 ? 4 : capacity() * 2;
			reserve(newcapacity);
		}
		/*for (int i = size(); i >pos; i--)
		{
			_start[i] = _start[i - 1];
		}*/
		iterator end = _finish;
		pos = _start + old_pos;
		while (end != pos)
		{
			*end = *(end - 1);
			--end;
		}
		++_finish;
		*pos = x;
		return pos;
	}
	template<class T>
	 typename vector<T>::iterator vector<T>::erase(iterator pos)
	{
		 assert(pos >= _start && pos < _finish);
		 iterator end = pos+1;
		 while (end != _finish)
		 {
			 *(end - 1) = *end;
			 ++end;
		 }
		 _finish--;
		 return pos;
	}

}



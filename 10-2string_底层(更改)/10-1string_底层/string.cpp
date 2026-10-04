#include"string.h"
#include<utility>

namespace bit
{
	// ==================== 构造 / 析构 / 赋值 ====================
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
		// 【清理】原代码残留调试输出,注释保留(不删):
		// cout << "	string::string(const string& s)" << endl;
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

	// ==================== reserve ====================
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

	// ==================== resize ====================
	// 【原代码-有误】整段注释保留(不删):
	// void string::resize(size_t n, char ch)
	// {
	// 	if (n > _size)
	// 	{
	// 		char* tmp = new char[n + 1];
	// 		strcpy(tmp, _str);
	// 		delete[]_str;
	// 		for (int i = _size; i < n; i++)
	// 		{
	// 			tmp[i] = '\0';    // 错1:应填参数 ch,不是 '\0'
	// 		}
	// 		_str = tmp;          // 错2:循环后没写 tmp[n] = '\0'
	// 		_size = n;
	// 		_capacity = n;       // 错3:n <= 旧容量时也重新分配,还把容量改小
	// 	}
	// 	if (n < _size)
	// 	{
	// 		char* tmp = new char[n + 1];
	// 		strcpy(tmp, _str);
	// 		delete[]_str;
	// 		_str = tmp;
	// 		_size = n;           // 错4:没写 tmp[n] = '\0',c_str 会读到旧数据
	// 	}
	// }
	// 【重写】1)只有 n > _capacity 才扩容;2)变大填 ch;3)末尾统一补 '\0'(变小时顺便完成截断)
	void string::resize(size_t n, char ch)
	{
		if (n > _capacity)
			reserve(n);

		if (n > _size)
		{
			for (size_t i = _size; i < n; ++i)
				_str[i] = ch;
		}

		_size = n;
		_str[_size] = '\0';
	}

	// ==================== push_back / append(你自己已改对,原样保留) ====================
	void string::push_back(char ch)
	{
		if (_size == _capacity)
		{
			reserve(_capacity == 0 ? 4 : _capacity * 2);
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
		strcpy(_str + _size, str);
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

	// ==================== insert(char) ====================
	// 【原代码-有误】整段注释保留(不删):
	// void string::insert(size_t pos, char ch)
	// {
	// 	if (_size == _capacity)
	// 	{
	// 		reserve(_capacity == 0 ? 4 : _capacity *= 2);  // 错1:先翻倍 _capacity 再传给 reserve,
	// 		//                                                reserve 里 n > _capacity 不成立,容量不会增长
	// 	}
	// 	size_t end = _size+1;
	// 	while (end > pos)
	// 	{
	// 		_str[end] = _str[end - 1];
	// 		--end;
	// 	}
	// 	_str[pos] = ch;
	// 	// 错2:缺 assert(pos <= _size)
	// 	// 错3:缺 ++_size
	// }
	// 【重写】
	void string::insert(size_t pos, char ch)
	{
		assert(pos <= _size);
		if (_size == _capacity)
		{
			reserve(_capacity == 0 ? 4 : 2 * _capacity);
		}

		// 从后往前,把 [pos, _size] 连同 '\0' 整体后移一格
		size_t end = _size + 1;
		while (end > pos)
		{
			_str[end] = _str[end - 1];
			--end;
		}
		_str[pos] = ch;
		++_size;
	}

	// ==================== insert(const char*) ====================
	// 【原代码-有误】整段注释保留(不删):
	// void string::insert(size_t pos, const char* str)
	// {
	// 	assert(pos <= _size);
	// 	size_t len = strlen(str);
	// 	if ((_size + len) > _capacity)
	// 	{
	// 		reserve(_size + len+1);  // +1 多余(reserve 内部会 +1),不算错
	// 	}
	// 	size_t end = _size + 1+len;
	// 	for (size_t i = _size + 1; i < pos; i--)  // 错1:初值 _size+1 恒 >= pos,循环体不执行,后半串没后移
	// 	{
	// 		_str[i + len - 1] = _str[i - 1];
	// 	}
	// 	for (size_t i = 0; i < len; i++)
	// 	{
	// 		_str[pos + i] = str[i];
	// 	}
	// 	_size += len;
	// 	_str[_size] = '\0';  // 错2:前面没后移成功,这里补 '\0' 也救不了
	// }
	// 【重写】
	void string::insert(size_t pos, const char* str)
	{
		assert(pos <= _size);
		size_t len = strlen(str);

		if ((_size + len) > _capacity)
		{
			reserve(_size + len);
		}

		// 从后往前,把 [pos, _size] 连同 '\0' 整体后移 len 位
		size_t end = _size + 1;
		while (end > pos)
		{
			_str[end + len - 1] = _str[end - 1];
			--end;
		}

		// 空出的 [pos, pos+len) 填入 str
		for (size_t i = 0; i < len; i++)
		{
			_str[pos + i] = str[i];
		}
		_size += len;
	}

	// ==================== erase ====================
	// 【原代码-有误】整段注释保留(不删):
	// void string::erase(size_t pos , size_t len )
	// {
	// 	// pos越界(原注释乱码,按意思重写)
	// 	if (len == string::npos || len > _size - pos)  // 错1:pos > _size 时 _size-pos 下溢
	// 	{
	// 		_str[pos] = '\0';
	// 		_size =pos;
	// 	}
	// 	else
	// 	{
	// 		for (size_t i = pos+len; i <_size; i++)
	// 		{
	// 			_str[pos++] = _str[i];
	// 		}
	// 		_size -= len;  // 错2:前移后没补 _str[_size] = '\0'
	// 	}
	// }
	// 【重写】
	void string::erase(size_t pos, size_t len)
	{
		assert(pos <= _size);

		if (len == string::npos || len > _size - pos)
		{
			// 删到末尾
			_str[pos] = '\0';
			_size = pos;
		}
		else
		{
			// [pos+len, _size] 连同 '\0' 整体前移 len 位
			for (size_t i = pos + len; i <= _size; i++)
			{
				_str[i - len] = _str[i];
			}
			_size -= len;
		}
	}

	// ==================== find ====================
	// 【原代码-有误】整段注释保留(不删):
	// size_t string::string:: find(char ch, size_t pos )const  // 错1:多了个 string::,语法错误
	// {
	// 	assert(pos > _size);  // 错2:断言写反了,pos 越界才该报错,应为 pos <= _size
	// 	for (size_t i = pos; i < _size; i++)
	// 	{
	// 		if (_str[i] == ch)
	// 		{
	// 			return i;
	// 		}
	// 	}
	// 	return npos;
	// }
	// 【重写】
	size_t string::find(char ch, size_t pos)const
	{
		assert(pos <= _size);
		for (size_t i = pos; i < _size; i++)
		{
			if (_str[i] == ch)
			{
				return i;
			}
		}
		return npos;

	}
	size_t string::find(const char* str, size_t pos)const
	{
		assert(pos <= _size);
		const char* ptr = strstr(_str + pos, str);
		if (ptr == nullptr)
		{
			return npos;
		}
		else
		{
			return ptr - _str;
		}
	}

	// ==================== substr ====================
	// 【原代码-有误】整段注释保留(不删):
	// string string::substr(size_t pos , size_t len )const
	// {
	// 	if (len == npos || len >= _size - pos)
	// 	{
	// 		len = _size - pos;
	// 	}
	// 	string sub;
	// 	sub.reserve(len );
	// 	for (int i = 0; i < _size; i++)  // 错1:应 i < len,否则把整个串都拷进去
	// 	{
	// 		sub += _str[pos + i];
	// 	}
	// 	return *this;  // 错2:应返回子串 sub,不是原串
	// }
	// 【重写】
	string string::substr(size_t pos, size_t len)const
	{
		assert(pos <= _size);
		if (len == npos || len >= _size - pos)
		{
			len = _size - pos;
		}
		string sub;
		sub.reserve(len);
		for (size_t i = 0; i < len; i++)
		{
			sub += _str[pos + i];
		}
		return sub;
	}

	// ==================== swap ====================
	// 【补充】原工程只有声明没有实现,调用会链接错误,这里补上:交换三个成员
	void string::swap(string& s)
	{
		std::swap(_str, s._str);
		std::swap(_size, s._size);
		std::swap(_capacity, s._capacity);
	}

	// ==================== npos 类外定义 ====================
	// 【补充】工程默认 C++14,npos 被 odr-use(return npos、len == string::npos 等)
	// 时需要类外定义,否则链接错误
	const size_t string::npos;
}

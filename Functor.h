#pragma once


template<typename T>
void	Safe_Delete(T& tmp)
{
	if (tmp)
	{
		delete tmp;
		tmp = nullptr;
	}
}

template<typename T>
void	Safe_Delete_Array(T& tmp)
{
	if (tmp)
	{
		delete[] tmp;
		tmp = nullptr;
	}
}

class CDeleteObj
{
public:
	template<typename T>
	void operator()(T& tmp)
	{
		if (tmp)
		{
			delete tmp;
			tmp = nullptr;
		}
	}
};

class CDeleteMap
{
public:
	template<typename T>
	void operator() (T& tmp)
	{
		if (tmp.second)
		{
			delete tmp.second;
			tmp.second = nullptr;
		}
	}
};

class CTagFinder
{
public:
	CTagFinder(const TCHAR* pTag)
		: m_pTag(pTag) { }

public:
	template<typename T>
	bool		operator()(T& Pair)
	{
		if (!lstrcmp(Pair.first, m_pTag)) // °°´Ù¸é
			return true;

		return false;
	}

private:
	const TCHAR* m_pTag;
};
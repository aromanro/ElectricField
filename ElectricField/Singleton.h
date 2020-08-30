#pragma once
template<class T> class Singleton
{
public:
	static T& getInstance()
	{
		static T t;

		return t;
	}
};


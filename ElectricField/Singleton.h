#pragma once

// I know, it's an anti pattern but it can be useful in certain cases
template<class T> class Singleton
{
public:
	static T& getInstance()
	{
		static T t;

		return t;
	}
};


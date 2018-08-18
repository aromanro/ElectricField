#include "stdafx.h"

#define _VECTOR_2D_IMPL

#include "Vector2D.h"

#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

template<typename T> Vector2D<T>::Vector2D() 
	: X(0), Y(0)
{
}

template<typename T> Vector2D<T>::Vector2D(const T& v)
	: X(v), Y(v)
{
}

template<typename T> template<typename O> Vector2D<T>::Vector2D(const Vector2D<O>& other)
	: X(other.X), Y(other.Y)
{
}

template<typename T> Vector2D<T>::Vector2D(const T& x, const T& y)
	: X(x), Y(y)
{
}


template<typename T> template<typename O> Vector2D<T>& Vector2D<T>::operator=(const Vector2D<O>& other) 
{
	X = other.X;
	Y = other.Y;

	return *this;
}


template<typename T> const Vector2D<T>& Vector2D<T>::operator+() const
{
	return *this;
}

template<typename T> Vector2D<T> Vector2D<T>::operator-() const
{
	return Vector2D<T>(-X, -Y);
}

template<typename T> template<typename O> Vector2D<T> Vector2D<T>::operator+(const Vector2D<O>& other) const
{
	return Vector2D<T>(X + other.X, Y + other.Y);
}

template<typename T> template<typename O> Vector2D<T> Vector2D<T>::operator-(const Vector2D<O>& other) const
{
	return Vector2D<T>(X - other.X, Y - other.Y);
}

template<typename T> template<typename O> T Vector2D<T>::operator*(const Vector2D<O>& other) const
{
	return X*other.X + Y*other.Y;
}

template<typename T> Vector2D<T> Vector2D<T>::operator*(T s) const
{
	return Vector2D<T>(X*s, Y*s);
}

template<typename T> Vector2D<T> Vector2D<T>::operator/(T s) const
{
	return Vector2D<T>(X/s, Y/s);
}

template<typename T> template<typename O> Vector2D<T>& Vector2D<T>::operator+=(const Vector2D<O>& other)
{
	return *this = *this + other;
}

template<typename T> template<typename O> Vector2D<T>& Vector2D<T>::operator-=(const Vector2D<O>& other)
{
	return *this = *this - other;
}

template<typename T> template<typename O> T Vector2D<T>::operator*=(const Vector2D<O>& other)
{
	return *this = *this * other;
}

template<typename T> Vector2D<T>& Vector2D<T>::operator*=(T s)
{
	return *this = *this * s;
}

template<typename T> Vector2D<T>& Vector2D<T>::operator/=(T s)
{
	return *this = *this / s;
}


template<typename T> double Vector2D<T>::Length() const
{
	const double w = *this * *this;

	return sqrt(w);
}

template<typename T> Vector2D<T> Vector2D<T>::Normalize() const
{
	const T len = Length();

	if (len != 0)
		return *this / len;

	return *this;
}

